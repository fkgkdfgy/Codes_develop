//
// Created by xiang on 1/4/18.
// this program shows how to perform direct bundle adjustment
//
#include <iostream>

#include "sophus/se3.hpp"
#include "Eigen/Eigen"
#include <opencv2/opencv.hpp>


#include <g2o/core/base_unary_edge.h>
#include <g2o/core/base_binary_edge.h>
#include <g2o/core/base_vertex.h>


#include <g2o/core/block_solver.h>
#include <g2o/core/optimization_algorithm_levenberg.h>
#include <g2o/core/optimization_algorithm_gauss_newton.h>
#include <g2o/core/optimization_algorithm_dogleg.h>
#include <g2o/solvers/dense/linear_solver_dense.h>
#include <g2o/solvers/eigen/linear_solver_eigen.h>
#include <g2o/core/robust_kernel_impl.h>


// #include <pangolin/pangolin.h>
#include <boost/format.hpp>

using namespace std;

typedef vector<Sophus::SE3d, Eigen::aligned_allocator<Sophus::SE3d>> VecSE3;
typedef vector<Eigen::Vector3d, Eigen::aligned_allocator<Eigen::Vector3d>> VecVec3d;

// global variables
string pose_file = "../poses.txt";
string points_file = "../points.txt";

// intrinsics
float fx = 277.34;
float fy = 291.402;
float cx = 312.234;
float cy = 239.777;

int border = 30;

// bilinear interpolation
inline float GetPixelValue(const cv::Mat &img, float x, float y) {
    uchar *data = &img.data[int(y) * img.step + int(x)];
    float xx = x - floor(x);
    float yy = y - floor(y);
    return float(
            (1 - xx) * (1 - yy) * data[0] +
            xx * (1 - yy) * data[1] +
            (1 - xx) * yy * data[img.step] +
            xx * yy * data[img.step + 1]
    );
}

class PointVertex : public g2o::BaseVertex<3, Eigen::Vector3d> {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    PointVertex() {}

    virtual void setToOriginImpl() override {
        _estimate = Eigen::Vector3d(0, 0, 0);
    }

    virtual void oplusImpl(const double *update) override {
        _estimate += Eigen::Vector3d(update[0], update[1], update[2]);
    }

    virtual bool read(istream &in) {}

    virtual bool write(ostream &out) const {}
};


// g2o vertex that use sophus::SE3 as pose
class VertexSophus : public g2o::BaseVertex<6, Sophus::SE3d> {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    VertexSophus() {}

    ~VertexSophus() {}

    bool read(std::istream &is) {}

    bool write(std::ostream &os) const {}

    virtual void setToOriginImpl() {
        _estimate = Sophus::SE3d();
    }

    Eigen::Vector2d project(const Eigen::Vector3d & pw) const
    {
        Eigen::Vector2d uv;
        Eigen::Vector3d pc = _estimate*pw;
        pc /= pc.z();
        uv<<fx*pc.x()+cx,fy*pc.y()+cy;
        return uv;
    }



    virtual void oplusImpl(const double *update_) {
        Eigen::Map<const Eigen::Matrix<double, 6, 1>> update(update_);
        setEstimate(Sophus::SE3d::exp(update) * estimate());
    }
};

// TODO edge of projection error, implement it
// 16x1 error, which is the errors in patch
typedef Eigen::Matrix<double,16,1> Vector16d;


class EdgeDirectProjection : public g2o::BaseBinaryEdge<16, Vector16d, VertexSophus, PointVertex> {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    EdgeDirectProjection(float *color, cv::Mat &target) {
        this->origColor = color;
        this->targetImg = target;
    }

    ~EdgeDirectProjection() {}

    virtual void computeError() override {
        int half_patch_size = 2;
        int count = 0;


        const PointVertex * pv = static_cast<const PointVertex *>(_vertices[1]);
        Eigen::Vector3d pw = pv->estimate();
        // std::cout<<_vertices.size()<<std::endl;
        const VertexSophus * posev = static_cast<const VertexSophus *>(_vertices[0]);
        Eigen::Vector2d uv = posev ->project(pw);

        if(uv.x() < border || uv.y()<border || uv.x() > targetImg.cols-border  || uv.y()> targetImg.rows-border)
        {
            _error.setZero();
        }
        else
        {
            for(int x= -half_patch_size ; x<half_patch_size;x++)
            {
                for (int y = -half_patch_size;y<half_patch_size;y++)
                {
                    _error(count) = origColor[count] - GetPixelValue(targetImg,uv.x(),uv.y());
                    count++;
                }
            }
        }

    }

    // Let g2o compute jacobian for you

    virtual bool read(istream &in) {}

    virtual bool write(ostream &out) const {}

private:    
    cv::Mat targetImg;  // the target image
    float *origColor = nullptr;   // 16 floats, the color of this point
};

// plot the poses and points for you, need pangolin
// void Draw(const VecSE3 &poses, const VecVec3d &points);

int main(int argc, char **argv) {

    // read poses and points
    VecSE3 poses;
    VecVec3d points;
    ifstream fin(pose_file);

    while (!fin.eof()) {
        double timestamp = 0;
        fin >> timestamp;
        if (timestamp == 0) break;
        double data[7];
        for (auto &d: data) fin >> d;
        poses.push_back(Sophus::SE3d(
                Eigen::Quaterniond(data[6], data[3], data[4], data[5]),
                Eigen::Vector3d(data[0], data[1], data[2])
        ));
        if (!fin.good()) break;
    }
    fin.close();


    vector<float *> color;
    fin.open(points_file);
    while (!fin.eof()) {
        double xyz[3] = {0};
        for (int i = 0; i < 3; i++) fin >> xyz[i];
        if (xyz[0] == 0) break;
        points.push_back(Eigen::Vector3d(xyz[0], xyz[1], xyz[2]));
        float *c = new float[16];
        for (int i = 0; i < 16; i++) fin >> c[i];
        color.push_back(c);

        if (fin.good() == false) break;
    }
    fin.close();

    cout << "poses: " << poses.size() << ", points: " << points.size() << endl;

    // read images
    vector<cv::Mat> images;
    boost::format fmt("../%d.png");
    for (int i = 0; i < 7; i++) {
        images.push_back(cv::imread((fmt % i).str(), 0));
    }

    // build optimization problem
    typedef g2o::BlockSolver<g2o::BlockSolverX> BlockSolverType;
    typedef g2o::LinearSolverDense<BlockSolverType::PoseMatrixType> LinearSolverType;
    // use LM
    auto solver = new g2o::OptimizationAlgorithmLevenberg(
        g2o::make_unique<BlockSolverType>(g2o::make_unique<LinearSolverType>()));

    g2o::SparseOptimizer optimizer;
    optimizer.setAlgorithm(solver);
    optimizer.setVerbose(true);

    std::vector<VertexSophus *> vertex_poses;
    std::vector<PointVertex *> vertex_pts;
    std::vector<EdgeDirectProjection * > edges;


    // TODO add vertices, edges into the graph optimizer
    // START YOUR CODE HERE
    //add vertex
    for (int i =0 ;i<poses.size();i++)
    {
        VertexSophus * vertex  = new VertexSophus();
        vertex->setEstimate(poses[i]);
        vertex->setId(i);
        optimizer.addVertex(vertex);
        vertex_poses.push_back(vertex);
    }

    for (int i =0 ;i<points.size();i++)
    {
        PointVertex * vertex  = new PointVertex();
        vertex->setEstimate(points[i]);
        vertex->setId(i+poses.size());
        optimizer.addVertex(vertex);
        vertex_pts.push_back(vertex);
    }

    //add edge
    for(int i =0;i<2;i++)
    {
        for(int j =0;j<color.size()/10;j++)
        {
            EdgeDirectProjection * edge = new EdgeDirectProjection(color[j],images[i]);
            edge->setVertex(1,vertex_pts[j]);
            edge->setVertex(0,vertex_poses[i]);
            edge->setInformation(Eigen::Matrix<double,16,16>::Identity());
            edge->setRobustKernel(new g2o::RobustKernelHuber());
            // std::cout<<optimizer.edges().size()<<endl;
            optimizer.addEdge(edge);
            edges.push_back(edge);
        }
    }

    // END YOUR CODE HERE

    // perform optimization
    optimizer.initializeOptimization(0);
    optimizer.optimize(20);

    // TODO fetch data from the optimizer
    // START YOUR CODE HERE
    for(int i=0;i<vertex_poses.size();i++)
    {
        poses[i] = vertex_poses[i]->estimate();
    }
    for(int i=0;i<vertex_pts.size();i++)
    {
        points[i] = vertex_pts[i]->estimate();
    }

    // END YOUR CODE HERE

    // plot the optimized points and poses
    // Draw(poses, points);

    // delete color data
    for (auto &c: color) delete[] c;
    return 0;
}

// void Draw(const VecSE3 &poses, const VecVec3d &points) {
//     if (poses.empty() || points.empty()) {
//         cerr << "parameter is empty!" << endl;
//         return;
//     }

//     // create pangolin window and plot the trajectory
//     pangolin::CreateWindowAndBind("Trajectory Viewer", 1024, 768);
//     glEnable(GL_DEPTH_TEST);
//     glEnable(GL_BLEND);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//     pangolin::OpenGlRenderState s_cam(
//             pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 389, 0.1, 1000),
//             pangolin::ModelViewLookAt(0, -0.1, -1.8, 0, 0, 0, 0.0, -1.0, 0.0)
//     );

//     pangolin::View &d_cam = pangolin::CreateDisplay()
//             .SetBounds(0.0, 1.0, pangolin::Attach::Pix(175), 1.0, -1024.0f / 768.0f)
//             .SetHandler(new pangolin::Handler3D(s_cam));


//     while (pangolin::ShouldQuit() == false) {
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         d_cam.Activate(s_cam);
//         glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

//         // draw poses
//         float sz = 0.1;
//         int width = 640, height = 480;
//         for (auto &Tcw: poses) {
//             glPushMatrix();
//             Sophus::Matrix4f m = Tcw.inverse().matrix().cast<float>();
//             glMultMatrixf((GLfloat *) m.data());
//             glColor3f(1, 0, 0);
//             glLineWidth(2);
//             glBegin(GL_LINES);
//             glVertex3f(0, 0, 0);
//             glVertex3f(sz * (0 - cx) / fx, sz * (0 - cy) / fy, sz);
//             glVertex3f(0, 0, 0);
//             glVertex3f(sz * (0 - cx) / fx, sz * (height - 1 - cy) / fy, sz);
//             glVertex3f(0, 0, 0);
//             glVertex3f(sz * (width - 1 - cx) / fx, sz * (height - 1 - cy) / fy, sz);
//             glVertex3f(0, 0, 0);
//             glVertex3f(sz * (width - 1 - cx) / fx, sz * (0 - cy) / fy, sz);
//             glVertex3f(sz * (width - 1 - cx) / fx, sz * (0 - cy) / fy, sz);
//             glVertex3f(sz * (width - 1 - cx) / fx, sz * (height - 1 - cy) / fy, sz);
//             glVertex3f(sz * (width - 1 - cx) / fx, sz * (height - 1 - cy) / fy, sz);
//             glVertex3f(sz * (0 - cx) / fx, sz * (height - 1 - cy) / fy, sz);
//             glVertex3f(sz * (0 - cx) / fx, sz * (height - 1 - cy) / fy, sz);
//             glVertex3f(sz * (0 - cx) / fx, sz * (0 - cy) / fy, sz);
//             glVertex3f(sz * (0 - cx) / fx, sz * (0 - cy) / fy, sz);
//             glVertex3f(sz * (width - 1 - cx) / fx, sz * (0 - cy) / fy, sz);
//             glEnd();
//             glPopMatrix();
//         }

//         // points
//         glPointSize(2);
//         glBegin(GL_POINTS);
//         for (size_t i = 0; i < points.size(); i++) {
//             glColor3f(0.0, points[i][2]/4, 1.0-points[i][2]/4);
//             glVertex3d(points[i][0], points[i][1], points[i][2]);
//         }
//         glEnd();

//         pangolin::FinishFrame();
//         usleep(5000);   // sleep 5 ms
//     }
// }

