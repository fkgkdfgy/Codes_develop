/**
 *           基本框架没有问题  现在主要问题是  Eigen的 Matrix 或者 Vector 存在 -nan 这种数值应该如何处理  
 * 
 * 
 * 
 * 
 * 
 */


#include "Eigen/Eigen"
#include <fstream>
#include <iostream>
#include <vector>

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"

namespace boost {
namespace serialization {

template<class Archive,typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
void serialize(Archive & ar, Eigen::Matrix<_Scalar,_Rows,_Cols,_Options,_MaxRows,_MaxCols> & mat, const unsigned int version)
{
    int cols, rows, type;
    bool continuous;
    if (Archive::is_saving::value) {
        cols = mat.cols(); rows = mat.rows();
    }

    ar&cols&rows;
    if(cols != mat.cols()|| rows!=mat.rows())
    {
        std::cerr<<"[FATAL]: trying to serialize data of different matrix!"<<std::endl;
        abort;
    }


    for(int i=0;i<cols*rows;i++)
    ar&mat.data()[i];
}   

} // namespace serialization
} // namespace boost

class EigenTestCase
{
    public:
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive & ar,const unsigned int version){
        int matrix_size, vector_size;
        if(Archive::is_saving::value)
        {
            matrix_size = vm33d_.size();
            vector_size = vv3d_.size();
        }

        ar & matrix_size & vector_size;
        
        if(!Archive::is_saving::value)
        {
            vm33d_.resize(matrix_size);
            vv3d_.resize(vector_size);
        }

        for(int i=0;i<vm33d_.size();i++)
        ar& vm33d_[i];
        for(int i =0;i<vv3d_.size();i++)
        ar&vv3d_[i];

        ar & m33d_;
    }

    std::vector<Eigen::Matrix3d> vm33d_;
    std::vector<Eigen::Vector3d> vv3d_;
    std::vector<int> vint_;
    Eigen::Matrix3d m33d_;


};  


int main()
{
    EigenTestCase eigen_test_case;

    // generate 内容

    for(int i = 0;i<10;i++)
    {
        eigen_test_case.vm33d_.push_back(Eigen::Matrix3d::Identity()*10);
        eigen_test_case.vv3d_.push_back(Eigen::Vector3d::Ones()*2333);
    }
    eigen_test_case.m33d_ = Eigen::Matrix3d::Identity() * 99;

    std::string filename = "/home/lwl/workspace/3rd-test-learning/11. boost-serialization/build/file";

    {
        std::ofstream ofs(filename.c_str());
        boost::archive::text_oarchive oa(ofs);
        oa<<eigen_test_case;
    }

    EigenTestCase new_eigen_test_case;
    {
        std::ifstream ifs(filename.c_str());
        boost::archive::text_iarchive ia(ifs);
        ia>>new_eigen_test_case;
        ifs.close();    
    }

    for(int i =0;i<10;i++)
    {
        std::cout<<new_eigen_test_case.vm33d_[i]<<std::endl;
        std::cout<<new_eigen_test_case.vv3d_[i]<<std::endl;
        
    }

    std::cout<<new_eigen_test_case.m33d_<<std::endl;

    // ofs.close();


    return 0;

}