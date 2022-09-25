/*
 * @Author: Liu Weilong
 * @Date: 2020-12-27 18:22:47
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-01-07 22:03:57
 * @FilePath: /3rd-test-learning/29. g2o/g2o_basic_example/g2o_powell_function.cpp
 * @Description: 
 * 
 * 
 *               
 */


#include "VE_self_powell_function.h"
#include "covariance_collector.h"



using namespace std;
int main()
{
    X * vx1 = new X();
    X * vx2 = new X();
    X * vx3 = new X();
    X * vx4 = new X();

    X4 * vx = new X4();
    
    typedef g2o::BlockSolver<g2o::BlockSolverTraits<4,10>> BlockSolverType;
    typedef g2o::LinearSolverDense<g2o::BlockSolverX::PoseMatrixType> LinearSolverType;
    
    // 创建求解器
    auto solver = new g2o::OptimizationAlgorithmDogleg(
        g2o::make_unique<g2o::BlockSolverX>(g2o::make_unique<LinearSolverType>()));
    g2o::SparseOptimizer optimizer;     // 图模型
    optimizer.setAlgorithm(solver);   // 设置求解器
    optimizer.setVerbose(true);       // 打开调试输出

    double dx1=3.0,dx2=-1.0,dx3=0.0,dx4=1.0;


    vx->setEstimate(Eigen::Vector4d(dx1,dx2,dx3,dx4));
    vx->setId(0);
    optimizer.addVertex(vx);

    F1 * ef1 = new F1();
    F2 * ef2 = new F2();
    F3F4 * ef3f4 = new F3F4();

    ef1->setVertex(0,vx);
    ef2->setVertex(0,vx);
    ef3f4->setVertex(0,vx);

  
    ef1->setInformation(Eigen::Matrix<double,1,1>::Identity());
    ef2->setInformation(Eigen::Matrix<double,1,1>::Identity());
    ef3f4->setInformation(Eigen::Matrix<double,2,2>::Identity());

    optimizer.addEdge(ef1);
    optimizer.addEdge(ef2);
    optimizer.addEdge(ef3f4);
    
    cout << "start optimization" << endl;
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();


    for(int i=0;i<20;i++)
    {
        optimizer.initializeOptimization();
        optimizer.optimize(1);
        cov_collector::CovCollector cc;
        cc.push_back(ef1,vx);
        cc.push_back(ef2,vx);
        cc.push_back(ef3f4,vx);

        Eigen::MatrixXd mm;
        cc.getHessian(vx,mm);
        cout<<"the hessian is "<<endl<<mm<<endl;
        cout<<"the det is "<<mm.determinant()<<endl;
    }
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    cout << "solve time cost = " << time_used.count() << " seconds. " << endl;

    cout<<"x1 is "<<vx1->estimate()<<endl;
    cout<<"x2 is "<<vx2->estimate()<<endl;
    cout<<"x3 is "<<vx3->estimate()<<endl;
    cout<<"x4 is "<<vx4->estimate()<<endl;
    
    // 用于读取 Hessian 矩阵
    // 原本这里 是用于得到 边缘化之后的 信息矩阵和向量， 用于之后用作先验的
    // 需要区别 现在版本中setMarginalized 中是 marginalized out 而不是留下
    // 使用单节点  也就是 v_x 会有报错的
    g2o::SparseBlockMatrix<Eigen::MatrixXd> info_matrix;
    std::vector<g2o::OptimizableGraph::Vertex*> v_x;
    v_x.push_back(vx);
    optimizer.computeMarginals(info_matrix,v_x);
    cout<<"the hessian idx is  "<<vx->hessianIndex()<<endl;
    cout<<"the hessian rows is "<<info_matrix.rows()<<endl;
    cout<<"the hessian cols is "<<info_matrix.cols()<<endl;
    // cout<<"the hessian matrix is "<< info_matrix.block(0,0)->eval()<<endl;
    cout<<"the estimation is "<<vx->estimate().transpose()<<endl;

    
    return 0;
}