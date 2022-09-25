/*
 * @Author: Liu Weilong
 * @Date: 2021-01-07 16:06:50
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-08 09:12:28
 * @FilePath: /3rd-test-learning/29. g2o/g2o_basic_example/covariance_collector.h
 * @Description: 
 * 
 *              这个类专门用于计算 协方差
 *              坑：
 *              1. BaseEdge 访问不到内部的 _vertices
 *              2. BaseEdge 内部的各种 _* 都是 protected 无法直接访问到内部
 *             
 *              2. Hessian  接近于0了,还是需要想办法验证CovCollector 的正确性
 *                 应该是对的
 * 
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "g2o/core/base_vertex.h"
#include "g2o/core/base_binary_edge.h"
#include "g2o/core/base_unary_edge.h"
#include "Eigen/Eigen"


#define _S_COV_COLLECTOR_ namespace cov_collector{
#define _E_COV_COLLECTOR_ };


_S_COV_COLLECTOR_

class CovCollector
{
    public:

    template<int D, typename E, typename VertexXi,typename T,int DV>
    void push_back(g2o::BaseUnaryEdge<D,E,VertexXi>* edge_ptr,
                   g2o::BaseVertex<DV,T> * vertex_ptr);
    
    template<int D, typename E, typename VertexXi, typename VertexXj,
             int DV1 ,typename T1,int DV2,typename T2>
    void push_back(g2o::BaseBinaryEdge<D,E,VertexXi,VertexXj>* edge_ptr,
                   g2o::BaseVertex<DV1,T1> * vertex_1_ptr,
                   g2o::BaseVertex<DV2,T2> * vertex_2_ptr);

    template<int D,typename T>
    bool getHessian(g2o::BaseVertex<D,T> * vertex_ptr,Eigen::MatrixXd & hessian);
    
    private:      
    std::vector<g2o::OptimizableGraph::Edge * > edge_pool_;
    std::vector<bool> type_pool_;
    std::map<g2o::OptimizableGraph::Vertex *,Eigen::MatrixXd> vertex_pool_;

};

template<int D, typename E, typename VertexXi,typename T,int DV>
void CovCollector::push_back(g2o::BaseUnaryEdge<D,E,VertexXi>* edge_ptr,
                             g2o::BaseVertex<DV,T> * vertex_ptr)
{
    auto tmp_edge_ptr = reinterpret_cast<g2o::OptimizableGraph::Edge *>(edge_ptr);
    edge_pool_.push_back(tmp_edge_ptr);
    edge_ptr->linearizeOplus();
    Eigen::Matrix<double,D,D> info =  edge_ptr->information();
    typename g2o::BaseUnaryEdge<D,E,VertexXi>::JacobianXiOplusType jacobian = edge_ptr->jacobianOplusXi();
    Eigen::Matrix<double,VertexXi::Dimension,VertexXi::Dimension> hessian;
    hessian = jacobian.transpose()*info*jacobian;

    auto tmp_vertex_ptr_1 = reinterpret_cast<g2o::OptimizableGraph::Vertex *>(vertex_ptr);
    if(!vertex_pool_.count(tmp_vertex_ptr_1))
    {
        vertex_pool_[tmp_vertex_ptr_1].resize(VertexXi::Dimension,VertexXi::Dimension);
        vertex_pool_[tmp_vertex_ptr_1].setZero();
    }

    // TODO 维数check
    vertex_pool_[tmp_vertex_ptr_1]+=hessian;
    type_pool_.push_back(0);
}

template<int D, typename E, typename VertexXi, typename VertexXj,
         int DV1 ,typename T1,int DV2,typename T2>
void CovCollector::push_back(g2o::BaseBinaryEdge<D,E,VertexXi,VertexXj>* edge_ptr,
                             g2o::BaseVertex<DV1,T1> * vertex_1_ptr,
                             g2o::BaseVertex<DV2,T2> * vertex_2_ptr)
{
    auto tmp_edge_ptr = reinterpret_cast<g2o::OptimizableGraph::Edge *>(edge_ptr);
    edge_pool_.push_back(tmp_edge_ptr);
    edge_ptr->linearizeOplus();

    Eigen::Matrix<double,D,D> info =  edge_ptr->information();

    typename g2o::BaseBinaryEdge<D,E,VertexXi,VertexXj>::JacobianXiOplusType jacobian1 = edge_ptr->jacobianOplusXi();
    Eigen::Matrix<double,VertexXi::Dimension,VertexXi::Dimension> hessian1;
    hessian1 = jacobian1.transpose()*info*jacobian1;

    auto tmp_vertex_ptr_1 = reinterpret_cast<g2o::OptimizableGraph::Vertex *>(vertex_1_ptr);
    if(!vertex_pool_.count(tmp_vertex_ptr_1))
    {
        vertex_pool_[tmp_vertex_ptr_1].resize(VertexXi::Dimension,VertexXi::Dimension);
        vertex_pool_[tmp_vertex_ptr_1].setZero();
    }
    vertex_pool_[tmp_vertex_ptr_1]+=hessian1;

    typename g2o::BaseBinaryEdge<D,E,VertexXi,VertexXj>::JacobianXjOplusType jacobian2 = edge_ptr->jacobianOplusXj();
    Eigen::Matrix<double,VertexXj::Dimension,VertexXj::Dimension> hessian2;
    hessian2 = jacobian2.transpose()*info*jacobian2;
    auto tmp_vertex_ptr_2 = reinterpret_cast<g2o::OptimizableGraph::Vertex *>(vertex_2_ptr);

    if(!vertex_pool_.count(tmp_vertex_ptr_2))
    {
        vertex_pool_[tmp_vertex_ptr_2].resize(VertexXi::Dimension,VertexXi::Dimension);
        vertex_pool_[tmp_vertex_ptr_2].setZero();
    }
    vertex_pool_[tmp_vertex_ptr_2]+=hessian2;
    
    type_pool_.push_back(1);
}

template<int D,typename T>
bool CovCollector::getHessian(g2o::BaseVertex<D,T> * vertex_ptr,Eigen::MatrixXd & hessian) 
{
    auto tmp_vertex_ptr_2 = reinterpret_cast<g2o::OptimizableGraph::Vertex *>(vertex_ptr);
    if(vertex_pool_.count(vertex_ptr))
    {
        hessian = vertex_pool_.at(vertex_ptr);
        return true;
    }
    return false;
}

_E_COV_COLLECTOR_

