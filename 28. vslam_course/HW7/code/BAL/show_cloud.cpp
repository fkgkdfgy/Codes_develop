/*
 * @Author: Liu Weilong
 * @Date: 2021-03-13 21:48:50
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-03-14 09:31:11
 * @Description: 
 */

#include <iostream>
#include <string>

#include "pcl/io/ply_io.h"
#include "pcl/conversions.h"
#include "pcl/visualization/cloud_viewer.h"



template<typename TPtr>
void SimpleShow(TPtr & point_cloud_)
{
    pcl::visualization::CloudViewer viewer ("Simple Cloud Viewer");
    viewer.showCloud (point_cloud_);
    while (!viewer.wasStopped ())
    {
    }
}

template<typename TPtrIn,typename TPtrOut>
void AssignColorToPointCloud(TPtrIn & pc_in, TPtrOut & pc_out, uint8_t r,uint8_t g,uint8_t b)
{
    pcl::copyPointCloud(*pc_in,*pc_out);
    for (int i =0;i<pc_out->size();i++)
    {
        uint32_t rgb = ((uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b);
        pc_out->points[i].rgb = *reinterpret_cast<float*>(&rgb);
    }
}

int main()
{
    std::string initial,final;
    initial = "/home/lwl/workspace/3rd-test-learning/28. vslam_course/HW7/code/BAL/build/initial.ply";
    final = "/home/lwl/workspace/3rd-test-learning/28. vslam_course/HW7/code/BAL/build/final.ply";

    pcl::PLYReader reader;
    pcl::PolygonMesh initial_mesh,final_mesh;
    pcl::PointCloud<pcl::PointXYZ>::Ptr initial_pts(new pcl::PointCloud<pcl::PointXYZ>()),
                                        final_pts(new pcl::PointCloud<pcl::PointXYZ>());
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr initial_rgb_pts(new pcl::PointCloud<pcl::PointXYZRGB>()),
                                      final_rbg_pts(new pcl::PointCloud<pcl::PointXYZRGB>());
    reader.read(initial,initial_mesh);
    reader.read(final,final_mesh);

    pcl::fromPCLPointCloud2(initial_mesh.cloud,*initial_pts);
    pcl::fromPCLPointCloud2(final_mesh.cloud,*final_pts);

    AssignColorToPointCloud(initial_pts,initial_rgb_pts,255,0,0);
    AssignColorToPointCloud(final_pts,final_rbg_pts,0,255,0);

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr all(new pcl::PointCloud<pcl::PointXYZRGB>());
    *all = *initial_rgb_pts + *final_rbg_pts;

    
    SimpleShow(all);
}