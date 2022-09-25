/*
 * @Author: Liu Weilong
 * @Date: 2021-05-11 10:12:58
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-03 22:25:38
 * @FilePath: /Codes/47. basalt/code/pose_graph_test.cc
 * @Description: 
 * 
 * 这个文件主要是为了测试 奇怪的位姿图优化
 * 
 * 
 * 具体公式见 doc.md 奇怪的位姿图优化求导 
 * 
 * 没有想到效果和正常版本基本一致
 * 
 * 真的是牛逼
 * 
 * 
 * 
 * 
 */
#include <iostream>
#include <fstream>
#include <string>
#include <Eigen/Core>

#include <g2o/core/base_vertex.h>
#include <g2o/core/base_binary_edge.h>
#include <g2o/core/block_solver.h>
#include <g2o/core/optimization_algorithm_levenberg.h>
#include <g2o/core/optimization_algorithm_gauss_newton.h>
#include <g2o/core/optimization_algorithm_dogleg.h>
#include <g2o/solvers/dense/linear_solver_dense.h>
#include <g2o/solvers/cholmod/linear_solver_cholmod.h>

#include "sophus/se3.hpp"
#include "sophus/so3.hpp"


using namespace std;

using Sophus::SE3d;
using Sophus::SO3d;

typedef Eigen::Matrix<double,6,6> Matrix6d;

// 给定误差求J_R^{-1}的近似
Matrix6d JRInv( SE3d e )
{
    Matrix6d J;
    J.block(0,0,3,3) = SO3d::hat(e.so3().log());
    J.block(0,3,3,3) = SO3d::hat(e.translation());
    J.block(3,0,3,3) = Eigen::Matrix3d::Zero(3,3);
    J.block(3,3,3,3) = SO3d::hat(e.so3().log());
    J = J*0.5 + Matrix6d::Identity();
    return J;
}
// 李代数顶点
typedef Eigen::Matrix<double, 6, 1> Vector6d;
class VertexSE3LieAlgebra: public g2o::BaseVertex<6, SE3d>
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    bool read ( istream& is )
    {
        double data[7];
        for ( int i=0; i<7; i++ )
            is>>data[i];
        setEstimate ( SE3d (
                Eigen::Quaterniond ( data[6],data[3], data[4], data[5] ),
                Eigen::Vector3d ( data[0], data[1], data[2] )
        ));
    }

    bool write ( ostream& os ) const
    {
        os<<id()<<" ";
        Eigen::Quaterniond q = _estimate.unit_quaternion();
        os<<_estimate.translation().transpose()<<" ";
        os<<q.coeffs()[0]<<" "<<q.coeffs()[1]<<" "<<q.coeffs()[2]<<" "<<q.coeffs()[3]<<endl;
        return true;
    }
    
    virtual void setToOriginImpl()
    {
        _estimate = Sophus::SE3d();
    }
    // 左乘更新
    virtual void oplusImpl ( const double* update )
    {
        Eigen::Map<const Eigen::Matrix<double,6,1>> up(update);
        
        _estimate = Sophus::SE3d::exp(up)*_estimate;
    }
};

// 两个李代数节点之边
class EdgeSE3LieAlgebra: public g2o::BaseBinaryEdge<6, SE3d, VertexSE3LieAlgebra, VertexSE3LieAlgebra>
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    bool read ( istream& is )
    {
        double data[7];
        for ( int i=0; i<7; i++ )
            is>>data[i];
        Eigen::Quaterniond q ( data[6], data[3], data[4], data[5] );
        q.normalize();
        setMeasurement (
            Sophus::SE3d ( q, Eigen::Vector3d ( data[0], data[1], data[2] ) ) 
        );
        for ( int i=0; i<information().rows() && is.good(); i++ )
            for ( int j=i; j<information().cols() && is.good(); j++ )
            {
                is >> information() ( i,j );
                if ( i!=j )
                    information() ( j,i ) =information() ( i,j );
            }
        return true;
    }
    bool write ( ostream& os ) const
    {
        VertexSE3LieAlgebra* v1 = static_cast<VertexSE3LieAlgebra*> (_vertices[0]);
        VertexSE3LieAlgebra* v2 = static_cast<VertexSE3LieAlgebra*> (_vertices[1]);
        os<<v1->id()<<" "<<v2->id()<<" ";
        SE3d m = _measurement;
        Eigen::Quaterniond q = m.unit_quaternion();
        os<<m.translation().transpose()<<" ";
        os<<q.coeffs()[0]<<" "<<q.coeffs()[1]<<" "<<q.coeffs()[2]<<" "<<q.coeffs()[3]<<" ";
        // information matrix 
        for ( int i=0; i<information().rows(); i++ )
            for ( int j=i; j<information().cols(); j++ )
            {
                os << information() ( i,j ) << " ";
            }
        os<<endl;
        return true;
    }

    // 误差计算与书中推导一致
    virtual void computeError()
    {
        Sophus::SE3d v1 = (static_cast<VertexSE3LieAlgebra*> (_vertices[0]))->estimate();
        Sophus::SE3d v2 = (static_cast<VertexSE3LieAlgebra*> (_vertices[1]))->estimate();
        _error = (_measurement.inverse()*v1.inverse()*v2).log();
    }
    
    // 雅可比计算
    virtual void linearizeOplus()
    {
        Sophus::SE3d v1 = (static_cast<VertexSE3LieAlgebra*> (_vertices[0]))->estimate();
        Sophus::SE3d v2 = (static_cast<VertexSE3LieAlgebra*> (_vertices[1]))->estimate();
        Matrix6d J = JRInv(SE3d::exp(_error));

        Sophus::SE3d R_m_R_1_T = _measurement.inverse()*v1.inverse();
        // 尝试把J近似为I？
        // _jacobianOplusXi = - J* v2.inverse().Adj();
        // _jacobianOplusXj = J*v2.inverse().Adj();

        // ! new add weird pose method
        _jacobianOplusXi = - R_m_R_1_T.Adj();
        _jacobianOplusXj = R_m_R_1_T.Adj();       
    }
};

int main ( int argc, char** argv )
{
    if ( argc != 2 )
    {
        cout<<"Usage: pose_graph_g2o_SE3_lie sphere.g2o"<<endl;
        return 1;
    }
    ifstream fin ( argv[1] );
    if ( !fin )
    {
        cout<<"file "<<argv[1]<<" does not exist."<<endl;
        return 1;
    }

    // typedef g2o::BlockSolver<g2o::BlockSolverTraits<6,6>> Block;  // BlockSolver为6x6
    // Block::LinearSolverType* linearSolver = new g2o::LinearSolverDense<Block::PoseMatrixType>(); // 线性方程求解器
    // Block* solver_ptr = new Block ( linearSolver );     // 矩阵块求解器
    // g2o::OptimizationAlgorithmLevenberg* solver = new g2o::OptimizationAlgorithmLevenberg ( solver_ptr );
    
    
    // ! new add
    typedef g2o::BlockSolver<g2o::BlockSolverTraits<6,6>> BlockSolverType;
    typedef g2o::LinearSolverCholmod<BlockSolverType::PoseMatrixType> LinearSolverType;
    
    // 创建求解器
    auto solver = new g2o::OptimizationAlgorithmLevenberg(
        g2o::make_unique<BlockSolverType>(g2o::make_unique<LinearSolverType>()));
    
    
    // 试试G-N或Dogleg？
    // g2o::OptimizationAlgorithmDogleg* solver = new g2o::OptimizationAlgorithmDogleg( solver_ptr );
    // g2o::OptimizationAlgorithmGaussNewton* solver = new g2o::OptimizationAlgorithmGaussNewton ( solver_ptr );
    
    g2o::SparseOptimizer optimizer;     // 图模型
    optimizer.setAlgorithm ( solver );  // 设置求解器

    int vertexCnt = 0, edgeCnt = 0; // 顶点和边的数量
    
    vector<VertexSE3LieAlgebra*> vectices;
    vector<EdgeSE3LieAlgebra*> edges;
    while ( !fin.eof() )
    {
        string name;
        fin>>name;
        if ( name == "VERTEX_SE3:QUAT" )
        {
            // 顶点
            VertexSE3LieAlgebra* v = new VertexSE3LieAlgebra();
            int index = 0;
            fin>>index;
            v->setId( index );
            v->read(fin);
            optimizer.addVertex(v);
            vertexCnt++;
            vectices.push_back(v);
            if ( index==0 )
                v->setFixed(true);
        }
        else if ( name=="EDGE_SE3:QUAT" )
        {
            // SE3-SE3 边
            EdgeSE3LieAlgebra* e = new EdgeSE3LieAlgebra();
            int idx1, idx2;     // 关联的两个顶点
            fin>>idx1>>idx2;
            e->setId( edgeCnt++ );
            e->setVertex( 0, optimizer.vertices()[idx1] );
            e->setVertex( 1, optimizer.vertices()[idx2] );
            e->read(fin);
            optimizer.addEdge(e);
            edges.push_back(e);
        }
        if ( !fin.good() ) break;
    }

    cout<<"read total "<<vertexCnt<<" vertices, "<<edgeCnt<<" edges."<<endl;

    cout<<"prepare optimizing ..."<<endl;
    optimizer.setVerbose(true);
    optimizer.initializeOptimization();
    cout<<"calling optimizing ..."<<endl;
    optimizer.optimize(30);

    cout<<"saving optimization results ..."<<endl;
    // 因为用了自定义顶点且没有向g2o注册，这里保存自己来实现
    // 伪装成 SE3 顶点和边，让 g2o_viewer 可以认出
    ofstream fout("result_lie.g2o");
    for ( VertexSE3LieAlgebra* v:vectices )
    {
        fout<<"VERTEX_SE3:QUAT ";
        v->write(fout);
    }
    for ( EdgeSE3LieAlgebra* e:edges )
    {
        fout<<"EDGE_SE3:QUAT ";
        e->write(fout);
    }
    fout.close();
    return 0;
}
