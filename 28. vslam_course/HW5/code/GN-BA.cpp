//
// Created by xiang on 12/21/17.
//
/*

    之前的bug 好像已经找到了
    原因1：
    在p3d点到p2d 投影的时候，忘记进行归一化了
    最后发现的确是这个问题




*/
#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;

#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "sophus/se3.hpp"

using namespace std;

typedef vector<Vector3d, Eigen::aligned_allocator<Vector3d>> VecVector3d;
typedef vector<Vector2d, Eigen::aligned_allocator<Vector3d>> VecVector2d;
typedef Matrix<double, 6, 1> Vector6d;

string p3d_file = "/home/lwl/workspace/3rd-test-learning/28. vslam_course/HW5/code/p3d.txt";
string p2d_file = "/home/lwl/workspace/3rd-test-learning/28. vslam_course/HW5/code/p2d.txt";

void Load2d(std::string path, VecVector2d & result);
void Load3d(std::string path, VecVector3d & result);


int main(int argc, char **argv) {

    VecVector2d p2d;
    VecVector3d p3d;
    Matrix3d K;
    double fx = 520.9, fy = 521.0, cx = 325.1, cy = 249.7;
    K << fx, 0, cx, 0, fy, cy, 0, 0, 1;

    // load points in to p3d and p2d 
    // START YOUR CODE HERE    
    Load2d(p2d_file,p2d);
    Load3d(p3d_file,p3d);


    // END YOUR CODE HERE
    assert(p3d.size() == p2d.size());

    int iterations = 100;
    double cost = 0, lastCost = 0;
    int nPoints = p3d.size();
    cout << "points: " << nPoints << endl;

    Sophus::SE3d T_esti=Sophus::SE3d::exp(Eigen::Matrix<double,6,1>::Zero()); // estimated pose

    std::cout<<"the init transform is "<<endl<<T_esti.matrix()<<endl;

    for (int iter = 0; iter < iterations; iter++) {

        Matrix<double, 6, 6> H = Matrix<double, 6, 6>::Zero();
        Vector6d b = Vector6d::Zero();

        cost = 0;
        // compute cost
        for (int i = 0; i < nPoints; i++) {
        // compute cost for p3d[I] and p2d[I]
        // START YOUR CODE HERE 
            Eigen::Vector3d p = T_esti*p3d[i];
            Eigen::Vector2d p2 = K.block<2,3>(0,0)*(p/p.z());
            // cout<<"K:"<<endl<<K.block<2,3>(0,0)<<endl;
            // cout<<"p2:"<<endl<<p2.transpose()<<endl;
            Eigen::Vector2d e = p2d[i] - p2;
            // Eigen::Vector2d e = p2d[i] - proj;
            cost += e.dot(e);

	    // END YOUR CODE HERE

	    // compute jacobian
            Matrix<double, 2, 6> J;

        // START YOUR CODE HERE 

            Eigen::Matrix<double,2,3> J_uv_p;
            Eigen::Matrix<double,3,6> J_p_xi;

            J_uv_p<< fx/p.z(),0,-1*fx*p.x()/p.z()/p.z(),
                     0,fy/p.z(),-1*fy*p.y()/p.z()/p.z();

            J_p_xi.block(0,0,3,3) = Eigen::Matrix<double,3,3>::Identity();
            J_p_xi.block(0,3,3,3) = -1*Sophus::SO3<double>::hat(p);
            
            J = -1*J_uv_p * J_p_xi;

	    // END YOUR CODE HERE

            H += J.transpose() * J;
            b += -J.transpose() * e;
        }

	// solve dx 
        Vector6d dx;

        // START YOUR CODE HERE 
        dx = H.ldlt().solve(b);
        
        // END YOUR CODE HERE

        if (isnan(dx[0])) {
            cout << "result is nan!" << endl;
            break;
        }

        if (dx.norm()<1e-4)
        {
            cout<<"converge!"<<endl;
            break;
        }

        // if (iter > 0 && cost >= lastCost) {
        //     // cost increase, update is not good
        //     cout << "cost: " << cost << ", last cost: " << lastCost << endl;
        //     break;
        // }

        // update your estimation
        // START YOUR CODE HERE 
        
        T_esti = Sophus::SE3d::exp(dx)*T_esti;

        // END YOUR CODE HERE
        
        lastCost = cost;

        cout << "iteration " << iter << " cost=" << cout.precision(12) << cost << endl;
    }

    cout << "estimated pose: \n" << T_esti.matrix() << endl;
    return 0;
}

void Load2d(std::string path, VecVector2d & result)
{
    std::ifstream fin(path.c_str());
    if(!fin.is_open())
    {
        cerr<<"the path is wrong"<<endl;
        abort();
    }
    string s,info;
    stringstream ss;
    std::vector<double> xy;
    while(getline(fin,s))
    {
        ss.str(s);
        while(getline(ss,info,' '))                           // 使用 ‘，’对一行数据进行分割
        {
            xy.push_back(atof(info.c_str())) ;         
        }
        Eigen::Vector2d p(xy[0],xy[1]);
        result.push_back(p);
        xy.clear();
        ss.clear();
    }
}

void Load3d(std::string path, VecVector3d & result)
{
    std::ifstream fin(path.c_str());
    if(!fin.is_open())
    {
        cerr<<"the path is wrong"<<endl;
        abort();
    }
    string s,info;
    stringstream ss;
    std::vector<double> xy;
    while(getline(fin,s))
    {
        ss.str(s);
        while(getline(ss,info,' '))                           // 使用 ‘，’对一行数据进行分割
        {
            xy.push_back(atof(info.c_str())) ;         
        }
        Eigen::Vector3d p(xy[0],xy[1],xy[2]);
        result.push_back(p);
        xy.clear();
        ss.clear();
    }
}