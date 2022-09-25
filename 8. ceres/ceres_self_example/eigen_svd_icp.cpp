/*
 * @Author: Liu Weilong
 * @Date: 2020-12-15 03:55:23
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-12-16 21:07:23
 * @Description: EigenSVDICP 类定义
 */

#include "eigen_svd_icp.h"

EigenSVDICP::EigenSVDICP(const std::string & config_path):
input_target_kdtree_(new pcl::KdTreeFLANN<PointType>())
{
    YAML::Node config = YAML::LoadFile(config_path.c_str());
    SetConfig(config);
}

bool EigenSVDICP::SetConfig(const YAML::Node & config_node)
{
    config_.max_iter_ = config_node["max_iter"].as<int>();
    config_.max_dist_ = config_node["max_dist"].as<double>();
    config_.euclidean_eps_ = config_node["euclidean_eps"].as<double>();
    config_.euclidean_change_ = config_node["euclidean_change"].as<double>();
    config_.trans_eps_ = config_node["trans_eps"].as<double>();
}

bool EigenSVDICP::SetInputTarget(const PointCloudPtr & input_target)
{
    input_target_kdtree_->setInputCloud(input_target);
    input_target_ = input_target;
    return true;
}

bool EigenSVDICP::ScanMatch(
    const PointCloudPtr & input_source,
    const Eigen::Matrix4f & predict_pose,
    PointCloudPtr & output_point_cloud,
    Eigen::Matrix4f & result_pose)
{
    input_source_ = input_source;

    PointCloudPtr transformed_input_source(new PointCloud());
    pcl::transformPointCloud(*input_source,*transformed_input_source,predict_pose);
    
    Eigen::Matrix4f transform;
    transform.setIdentity();
    
    for(int iter = 0;iter<config_.max_iter_;iter++)
    {
        PointCloudPtr curr_input_source(new PointCloud());
        pcl::transformPointCloud(*transformed_input_source,*curr_input_source,transform);

        std::vector<Eigen::Vector3f> xs;
        std::vector<Eigen::Vector3f> ys;
        if(GetCorrespondence(curr_input_source,xs,ys)<3)
        break;

        Eigen::Matrix4f delta_transform;
        GetTransform(xs,ys,delta_transform);
        
        if(!IsSignificant(delta_transform))
        break;

        transform = delta_transform * transform;
    }

    result_pose = (transform * predict_pose);

    pcl::transformPointCloud(*input_source_,*output_point_cloud,result_pose);

    return true;
}

size_t EigenSVDICP::GetCorrespondence(
    const PointCloudPtr &input_source, 
    std::vector<Eigen::Vector3f> &xs,
    std::vector<Eigen::Vector3f> &ys
) {
    const float MAX_CORR_DIST_SQR = config_.max_dist_ * config_.max_dist_;

    size_t num_corr = 0;

    for (size_t i = 0; i < input_source->points.size(); ++i) {
        std::vector<int> corr_ind;
        std::vector<float> corr_sq_dis;
        input_target_kdtree_->nearestKSearch(
            input_source->at(i), 
            1, 
            corr_ind, corr_sq_dis
        ); 

        if (corr_sq_dis.at(0) > MAX_CORR_DIST_SQR)
            continue;
        
        // add correspondence:
        Eigen::Vector3f x(
            input_target_->at(corr_ind.at(0)).x,
            input_target_->at(corr_ind.at(0)).y,
            input_target_->at(corr_ind.at(0)).z
        );
        Eigen::Vector3f y(
            input_source->at(i).x,
            input_source->at(i).y,
            input_source->at(i).z
        );

        xs.push_back(x);
        ys.push_back(y);

        ++num_corr;
    }

    return num_corr;
}

void EigenSVDICP::GetTransform(
    const std::vector<Eigen::Vector3f> &xs,
    const std::vector<Eigen::Vector3f> &ys,
    Eigen::Matrix4f &transformation
) {
    const size_t N = xs.size();

    // find centroids of mu_x and mu_y:
    Eigen::Vector3f mu_x = Eigen::Vector3f::Zero();
    Eigen::Vector3f mu_y = Eigen::Vector3f::Zero();
    for (size_t i = 0; i < N; ++i) {
        mu_x += xs.at(i);
        mu_y += ys.at(i);
    }
    mu_x /= N; 
    mu_y /= N;

    // build H:
    Eigen::Matrix3f H = Eigen::Matrix3f::Zero();
    for (size_t i = 0; i < N; ++i) {
        H += (ys.at(i) - mu_y) * (xs.at(i) - mu_x).transpose();
    }

    // solve R:
    Eigen::JacobiSVD<Eigen::MatrixXf> svd(H, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::Matrix3f R = svd.matrixV() * svd.matrixU().transpose();

    // solve t:
    Eigen::Vector3f t = mu_x - R * mu_y;

    // set output:
    transformation.setIdentity();
    transformation.block<3, 3>(0, 0) = R;
    transformation.block<3, 1>(0, 3) = t;
}

bool EigenSVDICP::IsSignificant(
    const Eigen::Matrix4f &transformation) 
{
    Eigen::Vector3f delta_trans = transformation.block<3,1>(0,3);
    float rotation_magnitude = fabs(
        acos(
            (transformation.block<3, 3>(0, 0).trace() - 1.0f) / 2.0f
        )
    );
    return (
    (delta_trans.norm() > config_.trans_eps_) || 
    (rotation_magnitude > config_.trans_eps_)
    );
}

