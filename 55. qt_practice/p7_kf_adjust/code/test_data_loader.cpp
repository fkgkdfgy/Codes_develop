#include "data_loader.h"

int main()
{
    KFAdjustDataLoader loader;
    loader.bag_path = "/home/lwl/gazebo_project_test";
    loader.Load(loader.bag_path);

    cout<<loader.pcd_name.front()<<endl;
    cout<<loader.pcd_name.back()<<endl;
    cout<<loader.kfs_pose.front().transpose()<<endl;
    cout<<loader.kfs_pose.back().transpose()<<endl;
}