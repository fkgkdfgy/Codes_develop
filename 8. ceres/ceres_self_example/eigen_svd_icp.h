/*
 * @Author: Liu Weilong
 * @Date: 2020-12-15 03:55:00
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-12-16 19:42:59
 * @Description: 
 */

#pragma once


#include <iostream>
#include <string>
#include <vector>

#include <Eigen/Eigen>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/common/transforms.h>
#include <yaml-cpp/yaml.h>


class EigenSVDICP
{
    public:
    
    using PointType = pcl::PointXYZ;
    using PointCloud = pcl::PointCloud<PointType>;
    using PointCloudPtr = PointCloud::Ptr;

    EigenSVDICP(const std::string & config_path);
    
    bool SetInputTarget(const PointCloudPtr & input_target);
    bool ScanMatch(
        const PointCloudPtr & input_source,
        const Eigen::Matrix4f & predict_pose,
        PointCloudPtr & output_point_cloud,
        Eigen::Matrix4f & result_pose
    );

    void GetTransform(
        const std::vector<Eigen::Vector3f> & xs,
        const std::vector<Eigen::Vector3f> & ys,
        Eigen::Matrix4f & transform
    );
    

    private:

    size_t GetCorrespondence(
        const PointCloudPtr & input_source,
        std::vector<Eigen::Vector3f> & xs,
        std::vector<Eigen::Vector3f> & ys
    );

    bool IsSignificant(
        const Eigen::Matrix4f & transform);

    bool SetConfig(const YAML::Node & config_node);

    class config
    {
        public:
        int max_iter_;
        double euclidean_eps_;
        double euclidean_change_;
        double max_dist_;
        double trans_eps_;
    };

    private:
    
    pcl::KdTreeFLANN<PointType>::Ptr input_target_kdtree_;
    PointCloudPtr input_source_;
    PointCloudPtr input_target_;
    config config_;
};

