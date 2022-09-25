/*
 * @Author: Liu Weilong
 * @Date: 2021-03-25 18:13:18
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-04-23 15:01:28
 * @FilePath: /Codes/38. line_feature/vanishing_point/code/environment_builder.h
 * @Description: 
 */

#include <iostream>
#include <vector>

#include "Eigen/Eigen"
#include "sophus/se3.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;

class BoxObservation;
class Line;
class ImageLine;
class ImageLineEigen;

ImageLineEigen toEigenLine(const ImageLine & input);
ImageLine toLine(const ImageLineEigen & input);

Eigen::Vector4d toV4d(const Eigen::Vector3d & p)
{
    return (Eigen::Vector4d()<<p,1.0).finished();
}

vector<Eigen::Vector4d> toV4d(const vector<Eigen::Vector3d> & vp)
{
    vector<Eigen::Vector4d> vp4d;
    vp4d.reserve(vp.size());
    for(auto & p:vp) vp4d.push_back(toV4d(p));
    return vp4d;
}

class Camera
{
    public:
    
    using M23 = Eigen::Matrix<double,2,3>;
    using M66 = Eigen::Matrix<double,6,6>;
    using M61 = Eigen::Matrix<double,6,1>;
    static double fx,fy,cx,cy;
    static double width,height;

    Sophus::SE3d T_w_c;

    Eigen::Matrix3d K()const;
    // Eigen::Matrix3d bigK()const;
    
    Eigen::Vector2d c2p(Eigen::Vector3d point) const ;
    Eigen::Vector3d p2c(Eigen::Vector2d px,double depth=1.0)const;  
    Eigen::Vector3d c2w(Eigen::Vector3d point) const;
    Eigen::Vector3d w2c(Eigen::Vector3d point) const;
    M23 J_uv_xyz(Eigen::Vector3d point)const;

};

double Camera::fx = 307.3070580996126;
double Camera::fy = 307.3070580996126;
double Camera::cx = 372.865217584225;
double Camera::cy = 240.0338623810578;
double Camera::width = 752;
double Camera::height = 480;

Eigen::Matrix3d Camera::K()const
{
    Eigen::Matrix3d output;
    output<<fx,0,cx,0,fy,cy,0,0,1;
    return output;
}

Eigen::Vector2d Camera::c2p(Eigen::Vector3d point) const
{
    Eigen::Vector2d px;
    point /= point.z();
    px << fx*point.x() + cx,
          fy*point.y() + cy;
    return px;
}

Eigen::Vector3d Camera::p2c(Eigen::Vector2d px,double depth) const
{
    Eigen::Vector3d point;
    point<< (px.x()-cx)/fx,
            (px.y()-cy)/fy,
            1.0;
    point *= depth;
    return point;
}

Eigen::Vector3d Camera::c2w(Eigen::Vector3d point_c) const
{
    Eigen::Vector3d p_w = T_w_c * point_c;
    return p_w;
}

Eigen::Vector3d Camera::w2c(Eigen::Vector3d point_w) const
{
    Eigen::Vector3d p_c = T_w_c.inverse() * point_w;
    return p_c;
}

Camera::M23 Camera::J_uv_xyz(Eigen::Vector3d point) const
{
    M23 result;
    double z_inv = 1./point.z();
    double z_inv_2 = z_inv*z_inv;

    result<< fx*z_inv,0.,-1.*fx*point.x()*z_inv_2,
             0.,fy*z_inv,-1.*fy*point.y()*z_inv_2;

    return result;
}



class Line
{
    public:
    Eigen::Vector3d start_point, end_point;
};

class Box
{
    public:
    
    static Box create(const Eigen::Vector3d & start,double length,cv::Scalar color = cv::Scalar(255,0,0));
    
    BoxObservation generateObs(Camera * cm);

    vector<Line> vl;
    vector<Line> x_axis_l;
    vector<Line> y_axis_l;
    vector<Line> z_axis_l;
    double length;
    Eigen::Vector3d start;
    cv::Scalar color;
};





class ImageLine
{
    public:
    cv::Point2f start_point,end_point;  
};

class ImageLineEigen
{
    public:
    Eigen::Vector2d start_point,end_point;
    Eigen::Vector3d getLine()const;
};


Eigen::Vector3d ImageLineEigen::getLine()const
{
    Eigen::Vector3d s; s<<start_point,1.0;
    Eigen::Vector3d e; e<<end_point,1.0;
    Eigen::Vector3d output = s.cross(e);
    return output;
}

Eigen::Vector3d getVPFromeLine(const ImageLineEigen & l1,const ImageLineEigen & l2)
{
    Eigen::Vector3d output = l1.getLine().cross(l2.getLine());
    return output;
}

Eigen::Vector3d getVLFromVP(const Eigen::Vector3d & vp1, const Eigen::Vector3d & vp2)
{
    Eigen::Vector3d output;
    output = vp1.cross(vp2);
    return output;
}

class BoxObservation
{
    public:

    vector<ImageLineEigen> v2dobs;

    vector<ImageLineEigen> v2dobs_x;
    vector<ImageLineEigen> v2dobs_y;
    vector<ImageLineEigen> v2dobs_z;
    
    vector<ImageLine>  vpixelobs_x;
    vector<ImageLine>  vpixelobs_y;
    vector<ImageLine>  vpixelobs_z;
    
    vector<ImageLine>  vpixelobs;
    cv::Scalar color;
    // sequence = 1 XY  sequence =2 YZ seuqnce 3 ZX
    Eigen::Vector3d toVL(const vector<ImageLineEigen> & source_1, const vector<ImageLineEigen> & source_2);
    Eigen::MatrixXd toEigenFormLine();
};

Eigen::MatrixXd BoxObservation::toEigenFormLine()
{
    Eigen::MatrixXd result(4,3);
    
    
}

Eigen::Vector3d BoxObservation::toVL(const vector<ImageLineEigen> & source_1, const vector<ImageLineEigen> & source_2)
{
    auto vp1 = getVPFromeLine(source_1[0],source_1[1]);
    auto vp2 = getVPFromeLine(source_2[0],source_2[1]);
    Eigen::Vector3d output;
    output = getVLFromVP(vp1,vp2);
    output.normalize();
    return output;
}

class BoxDisplayer
{
    public:
    vector<BoxObservation> vboxobs;
    // getLineObs 得到Box Displayer所有Box 的线观测
    // axis  =  1 -> X轴
    // axis  =  2 -> Y轴
    // axis  =  3 -> Z轴
    // Eigen::MatrixXd getLineObsX();
    cv::Mat showBox()const;
    cv::Mat showXYZ(bool vp=false)const;
};


cv::Mat BoxDisplayer::showBox()const
{
    cv::Mat img = cv::Mat::zeros(480,752,CV_8U);
    cv::cvtColor(img,img,CV_GRAY2RGB);

    for(auto & obs:vboxobs)
    {
        for(auto & l:obs.vpixelobs)
        {
            cv::line(img,l.start_point,l.end_point,obs.color,2,8);
        }
    }
    return img;
}



cv::Mat BoxDisplayer::showXYZ(bool vp)const
{
    cv::Mat img = cv::Mat::zeros(480,752,CV_8U);
    cv::cvtColor(img,img,CV_GRAY2RGB);

    cv::Scalar X (255,0,0);
    cv::Scalar Y (0,255,0);
    cv::Scalar Z (0,0,255);
    cv::Scalar VP(125,0,125);
    
    for(auto & obs:vboxobs)
    {
        for(auto & l:obs.vpixelobs_x)
        {
            cv::line(img,l.start_point,l.end_point,X,2,8);
            
            cv::Point2f scale = l.end_point -l.start_point;
            
            cv::Point2d vp_s = l.start_point + 200* scale;
            cv::Point2d vp_e = l.start_point - 200* scale;

            cv::line(img,vp_s,vp_e,VP,1,4);


            
        }
        for(auto & l:obs.vpixelobs_y)
        {
            cv::line(img,l.start_point,l.end_point,Y,2,8);
            cv::Point2f scale = l.end_point -l.start_point;
            
            cv::Point2d vp_s = l.start_point + 200* scale;
            cv::Point2d vp_e = l.start_point - 200* scale;

            cv::line(img,vp_s,vp_e,VP,1,4);
        }
        for(auto & l:obs.vpixelobs_z)
        {
            cv::line(img,l.start_point,l.end_point,Z,2,8);
            cv::Point2f scale = l.end_point -l.start_point;
            
            cv::Point2d vp_s = l.start_point + 200* scale;
            cv::Point2d vp_e = l.start_point - 200* scale;

            cv::line(img,vp_s,vp_e,VP,1,4);
        }
    }
    return img;
}

ImageLineEigen toEigenLine(const ImageLine & input)
{
    ImageLineEigen output;
    output.start_point.x() =  input.start_point.x;
    output.start_point.y() =  input.start_point.y;
    output.end_point.x() =  input.end_point.x;
    output.end_point.y() =  input.end_point.y;
    return output;
};


ImageLine toLine(const ImageLineEigen & input)
{
    ImageLine output;
    output.start_point.x = input.start_point.x();
    output.start_point.y = input.start_point.y();
    output.end_point.x = input.end_point.x();
    output.end_point.y = input.end_point.y();
    return output;
}
Box Box::create(const Eigen::Vector3d & start,double length,cv::Scalar color )
{
    Box output;
    output.length = length;
    output.start = start;
    output.color = color;
    
    Line l1,l2,l3,l4,l5,l6,l7,l8,l9,l10,l11,l12;
    l1.start_point = start;
    l1.end_point = start;
    l1.end_point.z() += length;

    l2.start_point = start;
    l2.end_point = start;
    l2.end_point.x() += length;

    l3.start_point = start;
    l3.end_point = start;
    l3.end_point.y() += length;

    l4.start_point = l1.end_point;
    l4.end_point = l1.end_point;
    l4.end_point.x() += length;
    
    l5.start_point = l1.end_point;
    l5.end_point = l1.end_point;
    l5.end_point.y() += length;

    l6.start_point = l2.end_point;
    l6.end_point = l2.end_point;
    l6.end_point.y() +=length;

    l7.start_point = l2.end_point;
    l7.end_point = l2.end_point;
    l7.end_point.z() +=length;

    l8.start_point = l3.end_point;
    l8.end_point = l3.end_point;
    l8.end_point.z() +=length;
    
    l9.start_point = l3.end_point;
    l9.end_point = l3.end_point;
    l9.end_point.x() +=length;

    l10.start_point = Eigen::Vector3d(length,length,length)+start;
    l10.end_point = Eigen::Vector3d(0,length,length)+start;

    l11.start_point = Eigen::Vector3d(length,length,length)+start;
    l11.end_point = Eigen::Vector3d(length,0,length)+start;

    l12.start_point = Eigen::Vector3d(length,length,length)+start;
    l12.end_point = Eigen::Vector3d(length,length,0)+start;

    output.vl.push_back(l1);
    output.vl.push_back(l2);
    output.vl.push_back(l3);
    output.vl.push_back(l4);
    output.vl.push_back(l12);
    output.vl.push_back(l11);
    output.vl.push_back(l10);
    output.vl.push_back(l9);
    output.vl.push_back(l8);
    output.vl.push_back(l7);
    output.vl.push_back(l6);
    output.vl.push_back(l5);

    output.z_axis_l.push_back(l1);
    output.z_axis_l.push_back(l12);
    output.z_axis_l.push_back(l7);
    output.z_axis_l.push_back(l8);

    output.x_axis_l.push_back(l4);
    output.x_axis_l.push_back(l10);
    output.x_axis_l.push_back(l9);
    output.x_axis_l.push_back(l2);

    output.y_axis_l.push_back(l3);
    output.y_axis_l.push_back(l6);
    output.y_axis_l.push_back(l5);
    output.y_axis_l.push_back(l11);



    return output;
}

BoxObservation Box::generateObs( Camera * cm)
{
    
    BoxObservation output;
    output.color = color;
    for(int i=0;i<vl.size();i++)
    {
        ImageLine t1;
        ImageLineEigen t2;
        t2.start_point = cm->c2p(cm->w2c(vl[i].start_point));
        t2.end_point = cm->c2p(cm->w2c(vl[i].end_point));
        t1 = toLine(t2);

        output.v2dobs.push_back(t2);
        output.vpixelobs.push_back(t1);
    }

    for(int i =0;i<x_axis_l.size();i++)
    {
        ImageLine t1;
        ImageLineEigen t2;
        t2.start_point = cm->c2p(cm->w2c(x_axis_l[i].start_point));
        t2.end_point = cm->c2p(cm->w2c(x_axis_l[i].end_point));
        t1 = toLine(t2);

        output.v2dobs_x.push_back(t2);
        output.vpixelobs_x.push_back(t1);
    }
    for(int i =0;i<y_axis_l.size();i++)
    {
        ImageLine t1;
        ImageLineEigen t2;
        t2.start_point = cm->c2p(cm->w2c(y_axis_l[i].start_point));
        t2.end_point = cm->c2p(cm->w2c(y_axis_l[i].end_point));
        t1 = toLine(t2);

        output.v2dobs_y.push_back(t2);
        output.vpixelobs_y.push_back(t1);
    }
    for(int i =0;i<z_axis_l.size();i++)
    {
        ImageLine t1;
        ImageLineEigen t2;
        t2.start_point = cm->c2p(cm->w2c(z_axis_l[i].start_point));
        t2.end_point = cm->c2p(cm->w2c(z_axis_l[i].end_point));
        t1 = toLine(t2);

        output.v2dobs_z.push_back(t2);
        output.vpixelobs_z.push_back(t1);
    }

    return output;
}

class Object
{
    public:
    Eigen::Quaterniond Camera;
    Eigen::Quaterniond Camera_MW;
    Eigen::Quaterniond Vehicle;
    std::vector<Eigen::Quaterniond> VehicleMotion;
    std::vector<Eigen::Quaterniond> CameraMotion;
    std::vector<Eigen::Quaterniond> MWMotion;
    Object(int i);
};

Object::Object(int i)
{
    Eigen::AngleAxisd R_vehicle_camera(0.3,Eigen::Vector3d(0,0.1,0.9).normalized());
    Eigen::AngleAxisd R_vehicle_MW(-0.4,Eigen::Vector3d(0.1,0.3,1.1).normalized());
    Camera = R_vehicle_camera;
    Camera_MW = R_vehicle_MW;
}

class Rotation
{
    public:
    static Rotation create(Eigen::Vector3d axis,double angle,double interval)
    {
        Rotation rot;
        for(double r = 0.0;r<angle;r+=interval)
        {
            Eigen::AngleAxisd aa(r,axis);
            rot.MotionArray.push_back(Eigen::Quaterniond(aa));
        }
        return rot;
    }  
    
    std::vector<Eigen::Quaterniond> MotionArray;
    
};
class TrajectoryEngine
{
    public:
    
    void addRotation(const Rotation & rot) {RotArray.push_back(rot);}
    // 运动均为右乘
    void addMotionForObject(Object & obj);
    std::vector<Rotation> RotArray;
};

void TrajectoryEngine::addMotionForObject(Object & obj)
{
    int size =0;
    for(auto & r:RotArray) size+= r.MotionArray.size();

    obj.VehicleMotion.clear();
    obj.VehicleMotion.reserve(size);
    obj.VehicleMotion.push_back(Eigen::Quaterniond(Eigen::Matrix3d::Identity()));

    // 给vehicle 进行赋值
    for(auto & r:RotArray)
    {
        Eigen::Quaterniond label_rot=obj.VehicleMotion.back();
        for(auto & mt:r.MotionArray)
        {
            obj.VehicleMotion.push_back(label_rot * mt);
        }
    }

    obj.CameraMotion.clear();
    obj.CameraMotion.reserve(size);
    obj.CameraMotion.push_back(Eigen::Quaterniond(Eigen::Matrix3d::Identity()));


    obj.MWMotion.clear();
    obj.MWMotion.reserve(size);
    obj.MWMotion.push_back(Eigen::Quaterniond(Eigen::Matrix3d::Identity()));

    for(int i=1;i<size;i++)
    {
        Eigen::Quaterniond relative_rot_v1_v2 = obj.VehicleMotion[i-1].inverse()*obj.VehicleMotion[i];
        Eigen::Quaterniond relative_rot_c1_c2 = obj.Camera.inverse()*relative_rot_v1_v2*obj.Camera;
        Eigen::Quaterniond relative_rot_mw1_mw2 = obj.Camera_MW.inverse()*relative_rot_v1_v2*obj.Camera_MW;

        obj.CameraMotion.push_back(obj.CameraMotion.back()*relative_rot_c1_c2);
        obj.MWMotion.push_back(obj.MWMotion.back()* relative_rot_mw1_mw2);
    }
}
