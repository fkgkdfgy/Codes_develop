/*
 * @Author: Liu Weilong
 * @Date: 2020-12-03 19:06:37
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-12-05 08:48:36
 * @Description: 
 */
#include "omp.h"
#include "Eigen/Eigen"
#include <iostream>
#include <string>
#include <map>
#include <utility>

#include "read_csv.h"
#include "common.h"

#define returnPtr(n)\
EigenDataUnit##n * returnPtr##n(){return ptr##n;}


using namespace std;

class DataCollector
{
    public:
    
    DataCollector(vector<string> & vPath , vector<string> & vName,int num = -1)
    {
        mvPath = vPath;
        mvName = vName;
        if(mvPath.size()!=vName.size())
        {
            cout<<"[WARNING]: the vPath size is not equal to vname's size."<<endl;
        }
        mvOriginData.resize(mvPath.size());
        #pragma omp parallel for
        for(int i =0;i<mvPath.size();i++)
        {
            vector<string> tmp_header;
            if(!readCSV(tmp_header,mvOriginData.at(i),mvPath.at(i),num))
            {
                std::cerr<<"read CSV failed!"<<endl;
                std::abort();
            }
            EigenDataPtr tmp_ptr = storeDataInEigenForm(mvOriginData.at(i));
            #pragma omp critical
            mmEigenFormData.insert(pair<string,EigenDataPtr>(mvName.at(i),tmp_ptr));
        }
    }
    
    // interface

    using DataUnit = vector<vector<double>>;
    using EigenDataUnit1 = vector<double>;
    using EigenDataUnit2 = vector<Eigen::Matrix<double,2,1>>;
    using EigenDataUnit3 = vector<Eigen::Matrix<double,3,1>>;
    using EigenDataUnit4 = vector<Eigen::Matrix<double,4,1>>;
    using EigenDataUnit5 = vector<Eigen::Matrix<double,5,1>>;

    class EigenDataPtr
    {
        public:

        // interface

        // internal
        returnPtr(1)
        returnPtr(2)
        returnPtr(3)
        returnPtr(4)
        returnPtr(5)

        EigenDataUnit1 * ptr1 = nullptr;
        EigenDataUnit2 * ptr2 = nullptr;
        EigenDataUnit3 * ptr3 = nullptr;
        EigenDataUnit4 * ptr4 = nullptr;
        EigenDataUnit5 * ptr5 = nullptr;
        int n = -1;
    };
    private:

    // internal 
    EigenDataPtr storeDataInEigenForm(DataUnit & du)
    {
        int N = du.size();
        EigenDataPtr tmp_ptr;
        switch (N)
        {
        case 1:{
            tmp_ptr.ptr1 = new EigenDataUnit1();
            tmp_ptr.n = 1;
            transformIntoVector(du,*tmp_ptr.ptr1);
            break;
        }

        case 2:{
            tmp_ptr.ptr2 = new EigenDataUnit2();
            tmp_ptr.n = 2;
            transformIntoVector(du,*tmp_ptr.ptr2);
            break;   
        }
        case 3:{
            tmp_ptr.ptr3 = new EigenDataUnit3();
            tmp_ptr.n = 3;
            transformIntoVector(du,*tmp_ptr.ptr3);
            break;   
        }
        case 4:{
            tmp_ptr.ptr4 = new EigenDataUnit4();
            tmp_ptr.n = 4;
            transformIntoVector(du,*tmp_ptr.ptr4);
            break;   
        }
        case 5:{
            tmp_ptr.ptr5 = new EigenDataUnit5();
            tmp_ptr.n = 5;
            transformIntoVector(du,*tmp_ptr.ptr5);
            break;   
        }
        default:
            break;
        }
        return tmp_ptr;
    }

    private: 
    vector<string> mvPath;
    vector<string> mvName;
    vector<DataUnit> mvOriginData;
    map<string,EigenDataPtr> mmEigenFormData;
};