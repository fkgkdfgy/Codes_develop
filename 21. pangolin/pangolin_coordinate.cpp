/*
 * @Author: Liu Weilong
 * @Date: 2020-12-23 09:22:21
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-12-23 09:40:22
 * @FilePath: /3rd-test-learning/24. pangolin/pangolin_coordinate.cpp
 * @Description:    Pangolin 的使用还是存在问题, 
 *                  有时间的时候要去看一下
 */
#include "pangolin/pangolin.h"

using namespace std;

int main()
{
    pangolin::CreateWindowAndBind("Line First",640,480);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    pangolin::OpenGlRenderState s_cam(
        pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 389, 0.1, 1000),
        pangolin::ModelViewLookAt(0, -0.1, -1.8, 0, 0, 0, 0.0, -1.0, 0.0)
    );

    pangolin::View &d_cam = pangolin::CreateDisplay()
        .SetBounds(0.0, 1.0, pangolin::Attach::Pix(175), 1.0, -1024.0f / 768.0f)
        .SetHandler(new pangolin::Handler3D(s_cam));
    
    while(!pangolin::ShouldQuit())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        d_cam.Activate(s_cam);
    
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glLineWidth(4);
        glColor3f(1.0f,0.0,0.0);
        glBegin(GL_LINE);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(4.0,4.0,4.0);

        glEnd();
        glLineWidth(4);
        glColor3f(1.0f,0.0,0.0);
        pangolin::glDrawLine(0.0,1.0,3.0,4.0);
        
        pangolin::FinishFrame();
    }

    return 0;
}
