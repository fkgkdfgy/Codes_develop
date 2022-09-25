/*
 * @Author: Liu Weilong
 * @Date: 2021-02-28 17:59:18
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-03-06 20:47:00
 * @Description: 
 * 
 * 进一步加速: 
 * 1. double 换成 float 应该基本能跑到opencv 的速度 为了精度这个不进行改进  并没有快多少 
 * 2. openmp 加速效果不明显  不知道为什么,可以看一下为什么没有加速           
 * 3. 换成 inverse 方法     在噪声的情况下，会出现不够稳定的情况，可以实现一下 40ms -> 24ms
 * 4. 换成 opencv 的 parallel_for_ 进行并行加速  可以实现一下             24ms ->  6ms
 * 5. Template 从 21降为 8                                            6ms ->  2ms
 * 5. 优化算法改一下 应该可以进一步加速                                  预计 2ms -> 1.4ms
 */


#include "tracker_base.h"
#include "common.h"
#include "omp.h"

#define OPENCV_PARALLEL

class LKtracker:public TrackerBase
{
   public:
   EIGEN_MAKE_ALIGNED_OPERATOR_NEW
   LKtracker(std::string & config,bool inverse):TrackerBase(config),inverse_(inverse){}
   virtual void Impl() override;
   
   private:
   void LKSingleLayer(const cv::Mat & pre_img,const cv::Mat & cur_img,
                      const Corners & pre_corners, Corners & cur_corners,
                      std::vector<bool> & success,
                      bool inverse = false, bool has_initial = false);
   bool SinglePixelOperation(const cv::Mat & pre_img,const cv::Mat & cur_img, 
                             const Eigen::Vector2f & origin, Eigen::Vector2f & cur_origin);
   std::vector<bool> success_;
   bool inverse_;
};

class ParallelPixelOperator
{
   public:

   ParallelPixelOperator(const cv::Mat & pre_img,const cv::Mat & cur_img,
                         const Corners & pre_corners,Corners & cur_corners,
                         std::vector<bool> & success,int half_patch_size,
                         bool inverse):
                        pre_img_(pre_img),cur_img_(cur_img),
                        pre_corners_(pre_corners),cur_corners_(cur_corners),
                        success_(success),half_patch_size_(half_patch_size),
                        inverse_(inverse){}
   
   void PixelOperation(const cv::Range & range);

   const cv::Mat & pre_img_;
   const cv::Mat & cur_img_;
   const Corners & pre_corners_;
   Corners & cur_corners_;
   std::vector<bool> & success_;
   int half_patch_size_;
   bool inverse_;
};


void LKtracker::Impl()
{
    Corners t_pre_corners,t_cur_corners;
    t_cur_corners.reserve(pre_corners_.size());
    t_pre_corners.reserve(pre_corners_.size());
    vector<float> scale_array;
    scale_array.reserve(options_ptr_->level_);
    scale_array.push_back(1.0);

    for(int i=1;i<options_ptr_->level_;i++)
        scale_array.push_back(scale_array[i-1]*options_ptr_->scale_);

    for(auto & kp:pre_corners_)
    {
        Eigen::Vector2f  p= kp*scale_array.back();
        t_pre_corners.push_back(p);
        t_cur_corners.push_back(p);
    }
    
    double larger = 1.0/options_ptr_->scale_;
    
    for(int i = scale_array.size()-1;i>=0;i--)
    {
        LKSingleLayer(pyr_pre_img_[i],pyr_cur_img_[i],t_pre_corners,t_cur_corners,success_);
        if(i!=0)
        {
            for(auto &p:t_pre_corners) p *= larger;
            for(auto &p:t_cur_corners) p *= larger;
        }
    }
    cur_corners_.reserve(t_cur_corners.size());
    for(auto & p :t_cur_corners)
    cur_corners_.push_back(p);
}

void LKtracker::LKSingleLayer(const cv::Mat & pre_img,const cv::Mat & cur_img,
                              const Corners & pre_corners, Corners & cur_corners,
                              std::vector<bool> & success,
                              bool inverse, bool has_initial)
{
    #ifdef OPENCV_PARALLEL
    success.resize(pre_corners.size(),false); 
    ParallelPixelOperator opt(pre_img,cur_img,pre_corners,cur_corners,success,options_ptr_->template_/2,inverse);
    cv::parallel_for_(cv::Range(0, pre_corners.size()),
                      std::bind(&ParallelPixelOperator::PixelOperation, &opt, placeholders::_1));
    #else
    for (int i = 0; i < pre_corners.size(); i++)
    {
        success[i] = SinglePixelOperation(pre_img,cur_img,pre_corners[i],cur_corners[i]);
    }
    #endif
}

// origin 是初始在cur_img 上的位置
bool LKtracker::SinglePixelOperation(const cv::Mat & pre_img,const cv::Mat & cur_img, 
                                     const Eigen::Vector2f & origin, Eigen::Vector2f & cur_origin)
{
    int half_path_size = options_ptr_->template_/2;
    int iterations = options_ptr_->max_iterations_;

    Eigen::Matrix2f H;
    Eigen::Vector2f b;
    Eigen::Vector2f J;
    Eigen::Vector2f temp_pos = cur_origin;

    float temp_x = temp_pos.x();
    float temp_y = temp_pos.y();
    float origin_x = origin.x();
    float origin_y = origin.y();
    float dx=0.,dy =0.,cost = 0.,lastCost =0.;
    std::vector<Eigen::Vector2f,Eigen::aligned_allocator<Eigen::Vector2f>> J_array;
    J_array.reserve(half_path_size *half_path_size);
    #define GetJ(x,y) J_array[y+half_path_size+(x+half_path_size)*half_path_size*2]

    
    // pixel postion check
    if(inverse_)
    {
        H.setZero();
        for(int x = -half_path_size;x<half_path_size;x++)
            for(int y = -half_path_size;y<half_path_size;y++){
            J = -1.0 * Eigen::Vector2f(
                0.5 *(GetPixelValue(pre_img,origin_x + x + 1,origin_y + y)-
                      GetPixelValue(pre_img,origin_x + x - 1,origin_y + y)),
                0.5 *(GetPixelValue(pre_img,origin_x + x,origin_y + y + 1)-
                      GetPixelValue(pre_img,origin_x + x,origin_y + y - 1))
                     );
            J_array.push_back(J);
            H += J *J.transpose();
            }
    }


    // 
    int i=0;
    for( i =0;i<iterations ; i++)
    {
        if(!inverse_)
        H.setZero();
        b.setZero();
        cost = 0;
        // 出 Jacobian 和 cost
        for(int x = -half_path_size;x<half_path_size;x++)
            for(int y = -half_path_size;y<half_path_size;y++)
            {
                float error = GetPixelValue(pre_img,origin_x+x,origin_y+y) - 
                              GetPixelValue(cur_img,temp_x+dx+x,temp_y+dy+y);
                if(!inverse_)
                    J = -1.0 * Eigen::Vector2f(
                    0.5 * (GetPixelValue(cur_img, temp_x+dx+x +1, temp_y+dy+y) -
                            GetPixelValue(cur_img, temp_x+dx+x -1, temp_y+dy+y)),
                    0.5 * (GetPixelValue(cur_img, temp_x+dx+x, temp_y+dy+y + 1) -
                            GetPixelValue(cur_img, temp_x+dx+x, temp_y+dy+y - 1))
                );   
                if(inverse_)
                b += -error * GetJ(x,y);
                else
                b += -error * J;
                
                cost += error * error;
                if(!inverse_)
                H += J * J.transpose();
            }
            Eigen::Vector2f update = H.ldlt().solve(b);

            if (std::isnan(update[0])) {
                // sometimes occurred when we have a black or white patch and H is irreversible
                cout << "update is nan" << endl;
                return false;
            }

            dx += update[0];
            dy += update[1];
            if (i > 0 && cost > lastCost) {
                break;
            }
            lastCost = cost;

            if (update.norm() < 1e-2) {
                break;
            }
    }

    if(i==iterations)
    return false;

    cur_origin.x() += dx;
    cur_origin.y() += dy;
    
    return true;
}

void ParallelPixelOperator::PixelOperation(const cv::Range & range)
{
    #define GetJ(x,y) J_array[y+half_patch_size_+(x+half_patch_size_)*half_patch_size_*2]

    int iterations = 20;
    for(size_t idx=range.start;idx<range.end;idx++)
    {
        Eigen::Matrix2f H;
        Eigen::Vector2f b;
        Eigen::Vector2f J;
        Eigen::Vector2f temp_pos = cur_corners_[idx];

        float temp_x = temp_pos.x();
        float temp_y = temp_pos.y();
        float origin_x = pre_corners_[idx].x();
        float origin_y = pre_corners_[idx].y();
        float dx=0.,dy =0.,cost = 0.,lastCost =0.;
        std::vector<Eigen::Vector2f,Eigen::aligned_allocator<Eigen::Vector2f>> J_array;
        J_array.reserve(half_patch_size_ *half_patch_size_);
       
        
        // pixel postion check
        if(inverse_)
        {
            H.setZero();
            for(int x = -half_patch_size_;x<half_patch_size_;x++)
                for(int y = -half_patch_size_;y<half_patch_size_;y++){
                J = -1.0 * Eigen::Vector2f(
                    0.5 *(GetPixelValue(pre_img_,origin_x + x + 1,origin_y + y)-
                        GetPixelValue(pre_img_,origin_x + x - 1,origin_y + y)),
                    0.5 *(GetPixelValue(pre_img_,origin_x + x,origin_y + y + 1)-
                        GetPixelValue(pre_img_,origin_x + x,origin_y + y - 1))
                        );
                J_array.push_back(J);
                H += J *J.transpose();
                }
        }


        // 
        int i=0;
        bool is_nan = false;
        for( i =0;i<iterations ; i++)
        {
            if(!inverse_)
            H.setZero();
            b.setZero();
            cost = 0;
            // 出 Jacobian 和 cost
            for(int x = -half_patch_size_;x<half_patch_size_;x++)
                for(int y = -half_patch_size_;y<half_patch_size_;y++)
                {
                    float error = GetPixelValue(pre_img_,origin_x+x,origin_y+y) - 
                                  GetPixelValue(cur_img_,temp_x+dx+x,temp_y+dy+y);
                    if(!inverse_)
                        J = -1.0 * Eigen::Vector2f(
                        0.5 * (GetPixelValue(cur_img_, temp_x+dx+x +1, temp_y+dy+y) -
                                GetPixelValue(cur_img_, temp_x+dx+x -1, temp_y+dy+y)),
                        0.5 * (GetPixelValue(cur_img_, temp_x+dx+x, temp_y+dy+y + 1) -
                                GetPixelValue(cur_img_, temp_x+dx+x, temp_y+dy+y - 1))
                    );   
                    if(inverse_)
                    b += -error * GetJ(x,y);
                    else
                    b += -error * J;
                    
                    cost += error * error;
                    if(!inverse_)
                    H += J * J.transpose();
                }
                Eigen::Vector2f update = H.ldlt().solve(b);

                if (std::isnan(update[0])) {
                    // sometimes occurred when we have a black or white patch and H is irreversible
                    is_nan = true;
                    break;
                }

                dx += update[0];
                dy += update[1];
                if (i > 0 && cost > lastCost) {
                    break;
                }
                lastCost = cost;

                if (update.norm() < 1e-2) {
                    break;
                }
        }

        if(i==iterations||is_nan == true){
        success_[idx] = false;
        continue;
        }

        cur_corners_[idx].x() += dx;
        cur_corners_[idx].y() += dy;
        
        success_[idx] = true;
    }
}