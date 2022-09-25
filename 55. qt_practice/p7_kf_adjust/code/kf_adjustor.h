#include "data_loader.h"
#include "pcl/visualization/pcl_visualizer.h"
#include "vtk-6.2/vtkWindow.h"

using PointT = pcl::PointXYZI;
using PointCloud = pcl::PointCloud<pcl::PointXYZI>;
using PointCloudPtr = pcl::PointCloud<pcl::PointXYZI>::Ptr;



class KFAdjustor
{
    public:
    KFAdjustor()
    {
        loader.bag_path = "/home/lwl/gazebo_project_test";
        loader.Load(loader.bag_path);
        viewer.reset(new pcl::visualization::PCLVisualizer());
        viewer->setBackgroundColor(0,0,0);
        viewer->setShowFPS(true);
        // viewer->registerMouseCallback(&KFAdjustor::mouse_callback,*this);
        viewer->registerPointPickingCallback(&KFAdjustor::pointpick_callback,*this);
        viewer->setCameraPosition(0, 0, 20, 1,0,0);
        viewer->registerKeyboardCallback(&KFAdjustor::keyboard_callback,*this);
    }
    // 展示所有KF组成的globalmap
    void showKFGlobalMap();
    
    // 主要目的是实现 wasd 进行点云移动
    void keyboard_callback(const pcl::visualization::KeyboardEvent & event, void * junk)
    {
        bool pc_move = false;
        
        // TODO 现在只有X轴上的平移操作 需要添加Y轴上平移操作，还有平移速度的操作
        if(event.getKeySym()=="w" && event.keyDown())
        {
            Eigen::Vector2d move(0.1,0.);
            selected_kf_info.pose.block<2,1>(0,0) += move;       
            pc_move = true;
        }

        if(event.getKeySym()=="s" && event.keyDown())
        {
            Eigen::Vector2d move(-0.1,0.);
            selected_kf_info.pose.block<2,1>(0,0) += move;
            pc_move = true;
        }

        if(event.getKeySym()=="a" && event.keyDown()&& !event.isAltPressed())
        {
            Eigen::Vector2d move(0.0,0.1);
            selected_kf_info.pose.block<2,1>(0,0) += move;
            
            pc_move = true;
        }
        if(event.getKeySym()=="d" && event.keyDown()&& !event.isAltPressed())
        {
            Eigen::Vector2d move(0.0,-0.1);
            selected_kf_info.pose.block<2,1>(0,0) +=  move;
            
            pc_move = true;
        }

        if(event.getKeySym()=="a" && event.keyDown()&& event.isAltPressed())
        {
            selected_kf_info.pose.z() += 0.05;   
            pc_move = true;
        }
        if(event.getKeySym()=="d" && event.keyDown()&& event.isAltPressed())
        {
            selected_kf_info.pose.z() -= 0.05;   
            pc_move = true;
        }
        
        if(pc_move)
        {
            PointCloudPtr tmp_pc(new PointCloud());
            Eigen::Matrix4f transform = GetTransformMatrixFromxytheta(selected_kf_info.pose.cast<float>());
            pcl::transformPointCloud(*selected_kf_info.point_cloud,*tmp_pc,transform);

            viewer->removePointCloud("point_cloud_"+to_string(selected_kf_info.id));
            viewer->removeShape("kf_sphere_"+to_string(selected_kf_info.id));
            viewer->removeShape("kf_line_x_"+to_string(selected_kf_info.id));
            viewer->removeShape("kf_line_y_"+to_string(selected_kf_info.id));


            viewer->addPointCloud<PointT>(tmp_pc,"point_cloud_"+to_string(selected_kf_info.id));
            viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "point_cloud_"+to_string(selected_kf_info.id));
		    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, 0, 1.0, 0, "point_cloud_"+to_string(selected_kf_info.id));

            Eigen::Vector3f t(selected_kf_info.pose.x(), selected_kf_info.pose.y(), 0);
            pcl::PointXYZ tp(t.x(),t.y(),t.z());

            viewer->addSphere(tp,0.1,"kf_sphere_"+to_string(selected_kf_info.id));
            viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, 0, 0, 1.0, "kf_sphere_"+to_string(selected_kf_info.id));

            Eigen::Matrix2d pose;
            Eigen::Vector2d move(0.3,0.0);
            pose(0, 0) = cos(selected_kf_info.pose.z());
            pose(0, 1) = -sin(selected_kf_info.pose.z());
            pose(1, 0) = sin(selected_kf_info.pose.z());
            pose(1, 1) = cos(selected_kf_info.pose.z());

            move = (pose * move);

            pcl::PointXYZ start(selected_kf_info.pose.x(),selected_kf_info.pose.y(),0.);
            pcl::PointXYZ endx(selected_kf_info.pose.x()+0.2,selected_kf_info.pose.y(),0.);
            pcl::PointXYZ endy(selected_kf_info.pose.x(),selected_kf_info.pose.y()+0.2,0.);

            viewer->addLine(start,endx,1.0,0,0,"kf_line_x_"+to_string(selected_kf_info.id));
            viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, 1.0, 0, 0, "kf_line_x_"+to_string(selected_kf_info.id));

            viewer->addLine(start,endy,1.0,0,0,"kf_line_y_"+to_string(selected_kf_info.id));
            viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, 0, 1.0, 0, "kf_line_y_"+to_string(selected_kf_info.id));


        }
    }

    // 实验证明 mouse_callback 只能得到二维渲染图像的 uv 像素坐标
    // 但是想要得到实际中的三维坐标
    void mouse_callback(const pcl::visualization::MouseEvent & event,void *junk)
    {
        if(event.getButton()== pcl::visualization::MouseEvent::LeftButton && 
           event.getType() == pcl::visualization::MouseEvent::MouseButtonPress)
           {
                cout<<"mouse press position:" << event.getX() << "  "<< event.getY()<<endl;

                viewer->getRenderWindow();

           }
    }
    
    void pointpick_callback(const pcl::visualization::PointPickingEvent & event, void * junk)
    {
        cout<<"pp_callback"<<endl;
        if (event.getPointIndex() == -1)
		return;
        float x,y,z;
        event.getPoint(x,y,z);
        cout<<"point pick :" <<x<<" "<<y<<" "<<z<<endl;
        cout<<"id"<<event.getPointIndex()<<endl;

        
        
    }
    
    pcl::visualization::PCLVisualizer::Ptr viewer;
    KFAdjustDataLoader loader;

    int selected_index = 0;
    bool point_cloud_move = true;
    KFInfo selected_kf_info;
};

void KFAdjustor::showKFGlobalMap()
{
    KFInfo info;
    int count = 0;
    while(loader.GetNextKFInfo(info)&& count<50)
    {
        count++;
        Eigen::Vector3f t(info.pose.x(), info.pose.y(), 0);
		Eigen::AngleAxisf aa(info.pose.z(), Eigen::Vector3f::UnitZ());
		Eigen::Quaternionf q(aa);

        pcl::PointXYZ position(t.x(),t.y(),t.z());

        // viewer->addText3D("kf_"+ to_string(info.id),position,0.02,1.0,0.,0.,"kf_text_"+to_string(info.id));
        viewer->addSphere(position,0.1,"kf_sphere_"+to_string(info.id));
        // viewer->addCube(t,q,0.1,0.1,0.1,"kf_pose_"+to_string(info.id));
        // viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_REPRESENTATION,
        // pcl::visualization::PCL_VISUALIZER_REPRESENTATION_SURFACE,"kf_pose_"+to_string(info.id));
        // viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, 1.0, 1.0, 1.0, "kf_pose_"+to_string(info.id));
        
        viewer->addPointCloud<PointT>(info.point_cloud,"point_cloud_"+to_string(info.id));
        viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1 ,"point_cloud_"+to_string(info.id));
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, 1.0, 1.0, 1.0, "point_cloud_"+to_string(info.id));
    }
    
    selected_index = info.id;

    viewer->removePointCloud("point_cloud_"+to_string(info.id));
    viewer->removeShape("kf_sphere_"+to_string(info.id));

    loader.GetRawDataKFInfoFromIndex(info.id,selected_kf_info);
}