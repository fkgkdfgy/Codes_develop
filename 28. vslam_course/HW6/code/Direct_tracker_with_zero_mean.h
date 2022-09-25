/*
 * @Author: Liu Weilong
 * @Date: 2021-03-03 09:50:42
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-24 17:17:47
 * @FilePath: /3rd-test-learning/28. vslam_course/HW6/code/Direct_tracker_with_zero_mean.h
 * @Description: 
 *  
 * 
 * 1. direct method 优化过程中float的精度 的确是有限制的 可能还是需要double
 *    先改成double吧
 * 2. 第二个怀疑是把一些边缘外的点加入了优化 发现是有一些这样的问题
 * 3. 一些bug 是自己写函数写了一部分没有写完就去作别的产生的
 * 4. 第三个bug 就是深度的问题
 * 5. 看看 inverse compostional + 并行能有多快
 * 
 * 
 * 
 * 
 * 
 */
#include "tracker_base.h"
#include "sophus/se3.hpp"
#include "common.h"


using namespace std;

class CameraInstrinc
{
    public:
    CameraInstrinc(const std::string & config){LoadInstrinc(config);}
    void LoadInstrinc(const std::string & config);
    void jaocobian_uv_xyz(const Eigen::Vector3d &p,Eigen::Matrix<double,2,3> & jacobian)const;
    Eigen::Vector2d c2p(const Eigen::Vector3d & c) const;
    Eigen::Vector3d p2c(const Eigen::Vector2d & p) const;
    
    CameraInstrinc & operator*(double scale);
    double fx,fy,cx,cy;
};

void CameraInstrinc::LoadInstrinc(const std::string & config)
{
    cv::FileStorage fin(config.c_str(),CV_STORAGE_READ);
    fx = double(fin["DirectTracker.Camera.fx"]);
    fy = double(fin["DirectTracker.Camera.fy"]);
    cx = double(fin["DirectTracker.Camera.cx"]);
    cy = double(fin["DirectTracker.Camera.cy"]);

    cout<<"DirectTracker.Camera.fx: "<<fx<<endl;
    cout<<"DirectTracker.Camera.fy: "<<fy<<endl;
    cout<<"DirectTracker.Camera.cx: "<<cx<<endl;
    cout<<"DirectTracker.Camera.cy: "<<cy<<endl;
}

void CameraInstrinc::jaocobian_uv_xyz(const Eigen::Vector3d &p, Eigen::Matrix<double,2,3> & jacobian) const
{
    jacobian.setZero();
    jacobian<<fx/p.z(),0,-1*fx*p.x()/p.z()/p.z(),
              0,fy/p.z(),-1*fy*p.y()/p.z()/p.z();
}

Eigen::Vector2d CameraInstrinc::c2p(const Eigen::Vector3d & p) const
{
    return Eigen::Vector2d(fx*p.x()/p.z()+cx,fy*p.y()/p.z()+cy);
}

Eigen::Vector3d CameraInstrinc::p2c(const Eigen::Vector2d & p) const
{
    return Eigen::Vector3d((p.x()-cx)/fx,(p.y()-cy)/fy,1.0);
}

CameraInstrinc & CameraInstrinc::operator*(double scale)
{
    this->fx *= scale;
    this->fy *= scale;
    this->cx *= scale;
    this->cy *= scale;
}

class DirectTracker:public TrackerBase
{
    public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    DirectTracker(std::string & config,bool inverse):TrackerBase(config),inverse_(inverse)
    {
        instrinc_ptr_ = new CameraInstrinc(config);
        result_.setZero();
    }
    virtual void Impl() override;
    void SetInput(const cv::Mat & pre_img){pre_img_ = pre_img.clone();}
    void SetPredict(const Eigen::Matrix<double,6,1> & predict){result_ = predict;}
    void SetLastFramePose(const Eigen::Matrix<double,6,1> & init){pre_pose_ = init;} 
    void SetLastFramePoints(const std::vector<Eigen::Vector3d,Eigen::aligned_allocator<Eigen::Vector3d>> & pre_pts,
                            const std::vector<bool> & good_pts);
    const CameraInstrinc * GetCamera()const {return instrinc_ptr_;} 
    const std::vector<bool> & GetGoodPts() const {return good_pts_array_;}
    const Eigen::Matrix<double,6,1> & GetResult()const {return result_;}

    /**
     *  param:
     *  @pts 应该是在世界坐标系下
     *  @pre_pose 应该是在世界坐标系下
     *  @result_pose 应该是在世界坐标系下
    */
    bool SingleLayerCalc(const cv::Mat & pre_img,const cv::Mat & cur_img,
                         const std::vector<Eigen::Vector3d,Eigen::aligned_allocator<Eigen::Vector3d>> & pts,
                         const Eigen::Matrix<double,6,1> & pre_pose,
                         Eigen::Matrix<double,6,1> & result_pose,  bool inverse);
                         
    Eigen::Matrix<double,6,1> pre_pose_;
    Eigen::Matrix<double,6,1> result_;
    CameraInstrinc * instrinc_ptr_;
    std::vector<Eigen::Vector3d,Eigen::aligned_allocator<Eigen::Vector3d>> pre_pts_; 
    std::vector<bool> good_pts_array_;
    bool success_;
    bool inverse_;
    float m = 0.;
};

void DirectTracker::SetLastFramePoints(const std::vector<Eigen::Vector3d,Eigen::aligned_allocator<Eigen::Vector3d>> & pre_pts,
                                       const std::vector<bool> & good_pts)
{
    pre_pts_ = pre_pts;
    good_pts_array_ = good_pts;

}

void DirectTracker::Impl()
{
    result_ = pre_pose_;
    std::vector<double> scale_array;
    scale_array.reserve(options_ptr_->level_);
    scale_array.push_back(1.0);
    
    for(int i=1;i<options_ptr_->level_;i++)
    {
        scale_array.push_back(scale_array[i-1]*options_ptr_->scale_);
    }

    instrinc_ptr_->operator*(scale_array.back());
    

    double larger = 1./options_ptr_->scale_;
    bool succ = false;
    for(int i=options_ptr_->level_-1;i>=0;i--)
    {
        succ = SingleLayerCalc(pyr_pre_img_[i],pyr_cur_img_[i],pre_pts_,pre_pose_,result_,false);
        if(succ == false) break;
        if(i!=0)
        instrinc_ptr_->operator*(larger);
    }
    if(succ==false) std::cout<<"not converge"<<std::endl;
}

bool DirectTracker::SingleLayerCalc(const cv::Mat & pre_img,const cv::Mat & cur_img,
                                    const std::vector<Eigen::Vector3d,Eigen::aligned_allocator<Eigen::Vector3d>> & pts,
                                    const Eigen::Matrix<double,6,1> & pre_pose,
                                    Eigen::Matrix<double,6,1> & result_pose,  bool inverse)
{
    // 初始化 H b J
    int half_patch_size = options_ptr_->template_/2;
    int itertations = 20;

    Eigen::Matrix<double,7,7> H;
    Eigen::Matrix<double,7,1> b;

    Eigen::Matrix<double,1,2> J_uv;
    Eigen::Matrix<double,2,3> J_xyz;
    Eigen::Matrix<double,3,6> J_xi;
    Eigen::Matrix<double,1,7> J;
    Eigen::Matrix<double,3,3> I = Eigen::Matrix<double,3,3>::Identity();

    Sophus::SE3d se3_pre = Sophus::SE3d::exp(pre_pose);
    Sophus::SE3d se3_cur = Sophus::SE3d::exp(result_pose);
    Sophus::SE3d d_se3 = Sophus::SE3d::exp(Eigen::Matrix<double,6,1>::Zero());

    Eigen::Vector3d p3d_pre,p3d_cur;
    
    double cost = 0.,Lastcost=0.,error=0.;

    int i;
    for(i=0;i<itertations;i++){

        H.setZero();
        b.setZero();
        cost=0.;
        
        for(int j=0;j<pts.size();j++)
        {
            const Eigen::Vector3d & p = pts[j];        
            // 没有写迭代
            
            p3d_pre = se3_pre*p;
            p3d_cur = d_se3*se3_cur*p;

            auto p2d_pre = instrinc_ptr_->c2p(p3d_pre);
            auto p2d_cur = instrinc_ptr_->c2p(p3d_cur);

            if(p2d_cur.x() < 0 || p2d_cur.y()<0 || p2d_cur.x() > cur_img.cols  || p2d_cur.y()> cur_img.rows ||good_pts_array_[j]==false)
            {
                good_pts_array_[j] = false;
                continue;
            }

            instrinc_ptr_->jaocobian_uv_xyz(p3d_cur,J_xyz);
            J_xi.block<3,3>(0,0) = I;
            J_xi.block<3,3>(0,3) = -1*Sophus::SO3d::hat(p3d_cur);
            
            Eigen::Matrix<double,2,6> J_uv_xi = J_xyz * J_xi;
                // 计算 Jacobian error
            for(int x = -half_patch_size; x <=half_patch_size ; x++)
                for(int y = -half_patch_size; y<=half_patch_size ; y++)
                {
                    error = GetPixelValue(pre_img,p2d_pre.x() + x,p2d_pre.y() + y) - 
                            GetPixelValue(cur_img,p2d_cur.x() + x,p2d_cur.y() + y) + m;
                    // jacobian 需要进一步进行检查
                    J_uv <<     -0.5 * (GetPixelValue(cur_img, p2d_cur.x() + x + 1, p2d_cur.y() + y) -
                                    GetPixelValue(cur_img, p2d_cur.x() + x - 1, p2d_cur.y() + y)),
                                -0.5 * (GetPixelValue(cur_img, p2d_cur.x() + x, p2d_cur.y() + y + 1) -
                                    GetPixelValue(cur_img, p2d_cur.x() + x, p2d_cur.y() + y - 1))
                            ;
                    J.block<1,6>(0,0) = J_uv*J_uv_xi;
                    J(0,6) = 1;
                    H += J.transpose() * J;
                    b += -1* error * J.transpose();
                    cost += error * error;
                }  
        } 
        
        // 更新 \delta \xi 
        Eigen::Matrix<double,7,1> update = H.ldlt().solve(b);
        Eigen::Matrix<double,6,1> update_so3 = update.block<6,1>(0,0);
        double update_m = update(6,0);


        if(std::isnan(update[0]))
        {
            std::cout<<" not converge !"<<endl;
            return false;
        }
        if(cost>Lastcost&&i!=0)
        {
            break;
        }
        if(update.norm()<3e-2)
        {
            break;
        }
        d_se3 = Sophus::SE3d::exp(update_so3)*d_se3;
        m += update_m;
        Lastcost = cost; 
    }

    result_pose = (d_se3*Sophus::SE3d::exp(result_pose)).log();
    return true;

}