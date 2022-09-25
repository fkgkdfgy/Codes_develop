/*
 * @Author: Liu Weilong
 * @Date: 2021-01-20 15:52:21
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-20 15:55:54
 * @FilePath: /3rd-test-learning/10. utils/feature_displayer/feature_displayer.h
 * @Description: 
 * 
 *                 从pointcloud_shower修改过来，主要是为了能够找到匹配管理 和 进一步的交互设计
 * 
 */

// TODO 还没有开始进行修改


#include <iostream>
#include <vector>

// unordered_map 无法使用的原因是因为没有对应的hash
// 所以这里借鉴pcl 使用uint32进行颜色的表示
#include <unordered_map>

#include "pangolin/pangolin.h"
#include "Eigen/Eigen"

#define _S_SLAM_DEMO_ namespace slam_demo {
#define _E_SLAM_DEMO_ };

_S_SLAM_DEMO_

class PangolinPointCloudShower
{
    public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    bool Show();

    // TODO 怀疑LoadPointCloud 对于不同的Allocator 会不会产生错误

    bool LoadPointCloud(Eigen::Vector3i color, std::vector<Eigen::Vector3d> & pts);
    bool ErasePointCloud(Eigen::Vector3i color);

    void SetPointCloud(const std::vector<Eigen::Matrix<double,3,1>> & points,Eigen::Vector3d color);
    bool ColorConvert(const Eigen::Vector3i color,uint32_t & color_ui32);
    bool ColorConvert(const uint32_t color_ui32,Eigen::Vector3i& color);
    private:
    std::unordered_map<uint32_t,std::vector<Eigen::Vector3d>> pts_pool_;

};

bool PangolinPointCloudShower::Show()
{

    if(pts_pool_.size()==0)
    return false;

    pangolin::CreateWindowAndBind("Main",640,480);
    glEnable(GL_DEPTH_TEST);

    // Define Projection and initial ModelView matrix
    pangolin::OpenGlRenderState s_cam(
        pangolin::ProjectionMatrix(640,480,420,420,320,240,0.2,100),
        pangolin::ModelViewLookAt(-2,2,-2, 0,0,0, pangolin::AxisY)
    );

    // Create Interactive View in window
    pangolin::Handler3D handler(s_cam);
    pangolin::View& d_cam = pangolin::CreateDisplay()
            .SetBounds(0.0, 1.0, 0.0, 1.0, -640.0f/480.0f)
            .SetHandler(&handler);
    
    // 更换背景颜色
    glClearColor(1.0f,1.0f,1.0f,1.0f); 
    while( !pangolin::ShouldQuit() )
    {
        // Clear screen and activate view to render into
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        d_cam.Activate(s_cam);
        
        for(auto & element:pts_pool_)
        {
            Eigen::Vector3i color;
            ColorConvert(element.first,color);
            SetPointCloud(element.second,color.cast<double>()/256.0);
        }
        // Swap frames and Process Events
        pangolin::FinishFrame();
    }
}

bool PangolinPointCloudShower::LoadPointCloud(Eigen::Vector3i color, std::vector<Eigen::Vector3d> & pts)
{
    uint32_t ui32;
    if(!ColorConvert(color,ui32))
    return false;
    for(auto & pt:pts)
    pts_pool_[ui32].push_back(pt);
    return true;
}

bool PangolinPointCloudShower::ErasePointCloud(Eigen::Vector3i color)
{
    uint32_t ui32;
    if(!ColorConvert(color,ui32))
    return false;
    pts_pool_.erase(ui32);
    return true;
}

void PangolinPointCloudShower::SetPointCloud(const std::vector<Eigen::Matrix<double,3,1>> & points,
                                             Eigen::Vector3d color)
{
    glColor3d(color.x(),color.y(),color.z());
    pangolin::glDrawPoints(points);
}
bool PangolinPointCloudShower::ColorConvert(const Eigen::Vector3i color,uint32_t & color_ui32 )
{
    if(color.maxCoeff()>256 || color.minCoeff()<0)
    return false;
    color_ui32 = ((uint32_t)color.x() << 16 | (uint32_t)color.y() << 8 | (uint32_t)color.z());
    return true;
}
bool PangolinPointCloudShower::ColorConvert(const uint32_t color_ui32,Eigen::Vector3i& color)
{
    color.x() = (int)(color_ui32&0x00FF0000)>>16;
    color.y() = (int)(color_ui32&0x0000FF00)>>8;
    color.z() = (int)(color_ui32&0x000000FF);
}
_E_SLAM_DEMO_