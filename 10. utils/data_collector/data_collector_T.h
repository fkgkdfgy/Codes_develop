/*
 * @Author: Liu Weilong
 * @Date: 2021-01-04 09:34:50
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-04 15:17:07
 * @FilePath: /3rd-test-learning/11. utils/data_collector/data_collector_T.h
 * @Description: 
 */

#include <vector>
#include <utility>
#include <algorithm>
#include <iomanip>


#include "Eigen/Eigen"
#include "data_collector_interface.h"

_S_DATA_COLLECTOR_

class DataCollectorT:public DataCollectorInterface<pair<int, Eigen::Matrix4d>>
{
    public:
    // 和Eigen 以及指针有关的错误，大多数是和 字节对齐有关
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    using Type = DataCollectorInterface::Type;
    using Suit = pair<int, Eigen::Matrix4d>;
    using RotVec = Eigen::Vector3d;
    using TransVec = Eigen::Vector3d;
    DataCollectorT(const string & store_path, const string & info_name):
    DataCollectorInterface(store_path,info_name){}
    
    DataCollectorT & operator<<(const Suit & info);
    
    virtual bool push_back(const Suit & input_info) override;    
    
    RotVec GetRotVec(const Suit & suit) const;
    TransVec GetTransVec(const Suit & suit) const;
    
    virtual ~DataCollectorT(){
        cout<<"the Data is destroying, save the data into "<< store_path_.c_str()<<endl;
        StoreInfo();     
        fout_.close();   
    }

    int ShowSize() const {return data_pool_.size();}


    private:

    virtual void StoreInfo() override ;
    
    private:
    using DataCollectorInterface::fout_;
    
    vector<Suit,Eigen::aligned_allocator<Suit>> data_pool_;
};  

_E_DATA_COLLECTOR_