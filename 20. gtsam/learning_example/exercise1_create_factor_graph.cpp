/*
 * @Author: Liu Weilong
 * @Date: 2020-10-28 14:09:00
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-10-31 20:53:11
 * @FilePath: /3rd-test-learning/22. gtsam/learning_example/exercise1_create_factor_graph.cpp
 * @Description:  
 *               学习一下如何创建一个 因子图
 */

#include "gtsam/nonlinear/NonlinearFactorGraph.h"
#include "gtsam/linear/NoiseModel.h"
#include "gtsam/geometry/Pose2.h"
#include "gtsam/slam/BetweenFactor.h"


using namespace gtsam;


int main()
{
    gtsam::NonlinearFactorGraph graph;
    
    // Add a Gaussian prior on pose x_1
    gtsam::Pose2 prior_mean(0.0,0.0,0.0);
    gtsam::noiseModel::Diagonal::shared_ptr prior_noise = 
    gtsam::noiseModel::Diagonal::Sigmas(gtsam::Vector3(0.3,0.3,0.1));

    graph.add(PriorFactor<Pose2>(1,prior_mean,prior_noise));

    Pose2 odometry(2.0,0.0,0.0);
    noiseModel::Diagonal::shared_ptr odometry_noise = 
    noiseModel::Diagonal::Sigmas(Vector3(0.2,0.2,0.1));

    graph.add(BetweenFactor<Pose2>(1,2,odometry,odometry_noise));
    graph.add(BetweenFactor<Pose2>(2,3,odometry,odometry_noise));
}