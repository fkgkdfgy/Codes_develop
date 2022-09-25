/*
 * @Author: Liu Weilong
 * @Date: 2020-12-24 13:57:00
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-07 15:38:38
 * @FilePath: /3rd-test-learning/29. g2o/g2o_basic_example/g2o_curvefitting.cpp
 * @Description: 
 * 
 *               关于使用多个CostFunction的问题
 * 
 *               BlockSolverTrait<3,1> 需要换成BlockSolverX
 * 
 */

#include "VE_self_curvefitting.h"

using namespace std;


int main()
{
    // 数据生成
    double ar = 1.0, br = 2.0, cr = 1.0;         // 真实参数值
    double ae = 2.0, be = -1.0, ce = 5.0;        // 估计参数值
    int N = 100;                                 // 数据点
    double w_sigma = 1e-1;                        // 噪声Sigma值
    double inv_sigma = 1.0 / w_sigma;
    cv::RNG rng;                                 // OpenCV随机数产生器

    std::vector<double> x_data, y_data;      // 数据
    for (int i = 0; i < N; i++) {
        double x = i / 100.0;
        x_data.push_back(x);
        y_data.push_back(exp(ar * x * x + br * x + cr) + rng.gaussian(w_sigma * w_sigma));
    }

    // trait 用于收集信息
    // 内部所有的 XXMatrix 都是Eigen::Matrix 的类型
    // 3是优化变量维数 1 是误差维数(来自 slambook2 但是这个说法并不正确)
    // 
    typedef g2o::BlockSolver<g2o::BlockSolverTraits<3,10>> BlockSolverType;
    typedef g2o::LinearSolverDense<BlockSolverType::PoseMatrixType> LinearSolverType;
    
    // 创建求解器
    auto solver = new g2o::OptimizationAlgorithmGaussNewton(
        g2o::make_unique<BlockSolverType>(g2o::make_unique<LinearSolverType>()));
    g2o::SparseOptimizer optimizer;     // 图模型
    optimizer.setAlgorithm(solver);   // 设置求解器
    optimizer.setVerbose(true);       // 打开调试输出

    // 添加顶点 这类涉及了 BaseVertex 的一些使用
    // setEstimate 给 _estimate赋值 
    // setId 用于设定id 号
    CurveFittingVertex *v = new CurveFittingVertex();
    v->setEstimate(Eigen::Vector3d(ae, be, ce));
    v->setId(0);
    // 以下两个属于这个优化问题不能使用的API
    // 一个是固定不变
    // 一个是优化之后边缘化掉这个顶点

    // v->setFixed(true);
    // v->setMarginalized(true);
    optimizer.addVertex(v);


    // setVertex 和相应的Vertex 产生连接关系
    // setId 设置边的序号
    // setMeasurement 设置观测类型
    // setInformation 设置信息矩阵
    for (int i = 0; i < N; i++) {
        CurveFittingEdge *edge = new CurveFittingEdge();
        edge->setId(i);
        edge->setVertex(0, v);                // 设置连接的顶点
        edge->setMeasurement(Eigen::Vector2d(x_data[i],y_data[i]));      // 观测数值
        edge->setInformation(Eigen::Matrix<double, 1, 1>::Identity() * 1 / (w_sigma * w_sigma)); // 信息矩阵：协方差矩阵之逆
        optimizer.addEdge(edge);
    }


    // 执行优化
    
    cout << "start optimization" << endl;
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    optimizer.initializeOptimization();
    optimizer.optimize(10);
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    cout << "solve time cost = " << time_used.count() << " seconds. " << endl;

    // 输出优化值
    Eigen::Vector3d abc_estimate = v->estimate();
    cout << "estimated model: " << abc_estimate.transpose() << endl;

    return 0;
}