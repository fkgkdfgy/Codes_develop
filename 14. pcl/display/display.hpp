/*
 * @Author: Liu Weilong
 * @Date: 2020-10-13 10:58:30
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-10-13 17:52:57
 * @Description:  这里SimpleShow 的想法就是只要给点附上颜色 放进viewer 就可以直接显示
 *                所以这里就暂时没有编写更复杂的 pcl_viewer
 */
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

