/*
 * @Author: Liu Weilong
 * @Date: 2021-05-01 20:55:46
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-03 09:06:03
 * @Description: 
 * 
 * 小优化器 
 * 目前，只支持 SE3 + Point3的使用
 * 目的是为了SE3 和Point3 的快速求解 也就是快速Schur 
 * 这里，基本使用的还是 g2o + ceres 的综合使用
 * 
 */
#include <iostream>

#include "sophus/se3.hpp"
/**
 * PoseVertex 内部使用 SE3 进行迭代更新
 * 因为SE3 的稳定性
*/

namespace little_g2o
{
        
    class BaseVertex
    {   
        public:
        virtual ~BaseVertex();
        virtual void Init(const double * data) =0;
        virtual void Update(const double * delta_x) =0;
        virtual void SetIdx(int idx){idx_ = idx;}
        const double * GetData() const { return data_;}
        double  * GetMutableData() const { return data_;}
        // 初始化检测
        bool init_ = false;
        int idx_ = 0;
        double * data_;
        int size_;
    };

    template<int N>
    class SizedVertex :public BaseVertex
    {
        public:
        SizedVertex()
        {
            if(N<=0) 
            {
                cerr<<"[FATAL]: Wrong Vertex Dimension"<<endl;
                abort();
            }
            data_ = new double[N];
            size_ = N;
        }
        ~SizedVertex(){delete [] data_;}

        virtual void Init(const double * data) = 0 ;
        virtual void Update(const double * data) = 0;
    };

    class PoseVertex: public SizedVertex<6>
    {
        public:
        
        virtual void Init(const double * data) override;
        virtual void Update(const double * data) override;
    };

    class PointVertex :public SizedVertex<3>
    {
        public:

        virtual void Init(const double * data) override;
        virtual void Update(const double * data) override;
    };

    class BaseEdge
    {
        public:
        virtual void Evaluate(const double **parameters, double * residual ,double ** jacobian) = 0;
        virtual void SetResidualSize(int n) {num_residual_ = n;}
        int num_residual_;
        std::vector<int> vertex_sizes_;
    };
    
    
    
};








