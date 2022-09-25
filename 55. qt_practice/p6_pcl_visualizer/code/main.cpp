/**
 * 主要目的是测试 pcl_visualizer 的使用
 * 1. 多种callback 的使用
 * 2. 给pcl_visualizer 添加各种点云
 * 3. AreaPicking
 * 
*/


#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "pcl/visualization/pcl_visualizer.h"

using namespace pcl::visualization;


PCLVisualizer viz;

// 每一次落下和抬起都是有Event 产生的
void keyboardEventOccurred(const pcl::visualization::KeyboardEvent& event, void* nothing){

    static double color = 1.0;
    
    if(event.getKeySym() == "l" && event.keyDown()){
        viz.removeAllShapes();
        viz.addCube(0,1.0,0,1.0,0,1.0);
        viz.setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_REPRESENTATION,
        pcl::visualization::PCL_VISUALIZER_REPRESENTATION_SURFACE,"cube");
        viz.setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, color, color, color, "cube");

    }
    color -= 0.05;
    cout<<"current color :"<< color<<endl;
    cout<<"getKeySym: " <<event.getKeySym()<<endl;
    cout<<"keyDown: "<<event.keyDown()<<endl;
}



int main()
{
    
    viz.setBackgroundColor(0,0,0);
    viz.setShowFPS(true);

    // 各种内容的设置
    viz.addCube(0,1.0,0,1.0,0,1.0);
    viz.setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_REPRESENTATION,
    pcl::visualization::PCL_VISUALIZER_REPRESENTATION_SURFACE,"cube");
	viz.setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, 1.0, 0.0, 0.0, "cube");

    // 各种callback

    viz.registerKeyboardCallback(keyboardEventOccurred);

viz.spin();

}