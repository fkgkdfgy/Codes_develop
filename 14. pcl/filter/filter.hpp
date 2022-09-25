/*
 * @Author: Liu Weilong
 * @Date: 2020-10-13 17:56:32
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-10-13 18:28:58
 * @Description: 编写 常用filter 的wrapper  
 *               这里要注意的是 typedef 其实给了我们一个可以访问 template的typename的方式
 *               具体内容可以见 PointCloud 的typename  
 *               所以在进行template 类编写的时候
 *               typedef 还是需要添加的
 */
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include "boost/shared_ptr.hpp"

template <typename T>
void VoxelFilter(boost::shared_ptr<T> & point_cloud_ptr,
                 boost::shared_ptr<T> & point_cloud_output_ptr,
                 float leaf_size_x,float leaf_size_y,float leaf_size_z)
{
    pcl::VoxelGrid<typename T::PointType> filter;
    filter.setInputCloud(point_cloud_ptr);
    filter.setLeafSize(leaf_size_x,leaf_size_y,leaf_size_z);
    filter.filter(*point_cloud_output_ptr);
}

template <typename T>
void VoxelFilter(boost::shared_ptr<T> & point_cloud_ptr,
                 boost::shared_ptr<T> & point_cloud_output_ptr,
                 float leaf_size)
{
    VoxelFilter<T>(point_cloud_ptr,point_cloud_output_ptr,leaf_size,leaf_size,leaf_size);
}


