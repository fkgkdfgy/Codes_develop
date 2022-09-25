/*
 * @Author: Liu Weilong
 * @Date: 2021-01-13 23:28:07
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-01-15 07:01:13
 * @Description: 
 */

#include <iostream>
#include <vector>

#include "Eigen/Eigen"
#include "pangolin/pangolin.h"

using namespace std;

std::vector<Eigen::Matrix<double,3,1>> line_1,line_2,line_3;



void ShowCude()
{
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
    while( !pangolin::ShouldQuit() )
    {
        // Clear screen and activate view to render into
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        d_cam.Activate(s_cam);

        // Render OpenGL Cube
        glColor3d(0.3,0.5,0.1);
        pangolin::glDrawCircle(1.0,2.0,5.0);
        // Swap frames and Process Events
        pangolin::FinishFrame();
    }
}

void SetPointCloud(const std::vector<Eigen::Matrix<double,3,1>> & points,Eigen::Vector3d color)
{
    glColor3d(color.x(),color.y(),color.z());
    pangolin::glDrawPoints(points);
}

void ShowPointCloud()
{
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

        SetPointCloud(line_1,Eigen::Vector3d(1.0,0.0,0.0));
        SetPointCloud(line_2,Eigen::Vector3d(0.0,1.0,0.0));
        SetPointCloud(line_3,Eigen::Vector3d(0.0,0.0,1.0));
        // Swap frames and Process Events
        pangolin::FinishFrame();
    }
}

int main()
{
    //ShowCude();
    
    for(double i=0;i<3;i=i+0.01)
    {
        line_1.emplace_back(i,i,i);
        line_2.emplace_back(i*2,i*3,i*6);
        line_3.emplace_back(-1*i,-2*i,-9*i);
    }
    
    ShowPointCloud();
    
    


    return 0;
}