/*
 * @Author: Liu Weilong
 * @Date: 2021-01-04 09:57:09
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-04 11:26:00
 * @FilePath: /3rd-test-learning/11. utils/data_collector/data_collector_T.cpp
 * @Description: 
 */
#include "data_collector_T.h"

_S_DATA_COLLECTOR_


bool SuitCompare(const DataCollectorT::Suit & a, const DataCollectorT::Suit & b) 
{
    return (a.first<b.first)?true:false;
}

DataCollectorT& DataCollectorT::operator<<(const Suit & info)
{
    data_pool_.push_back(info);
}
bool DataCollectorT::push_back(const Suit & info )
{
    data_pool_.push_back(info);
    return true;
}

void DataCollectorT::StoreInfo()
{
    sort(data_pool_.begin(),data_pool_.end(),SuitCompare);
    fout_<<"the header is "<<endl<<"the frameId: int "<<"RotVec: X Y Z "<<"         TransVec: X Y Z"<<endl;
    for(auto & suit:data_pool_)
    {

        fout_<< setprecision(4);
        fout_<< setw(7);
        fout_<< suit.first<<"  "
        <<setw(7)<< GetRotVec(suit).x()<<"  "
        <<setw(7)<< GetRotVec(suit).y()<<"  "
        <<setw(7)<< GetRotVec(suit).z()<<"  "
        <<setw(7)<< GetTransVec(suit).x()<<"  "
        <<setw(7)<< GetTransVec(suit).y()<<"  "
        <<setw(7)<< GetTransVec(suit).z()<<endl;
    }
}

DataCollectorT::RotVec DataCollectorT::GetRotVec(const Suit & suit)const
{
    const Eigen::Matrix4d & T = suit.second;
    Eigen::AngleAxisd angleAndaxis (T.block<3,3>(0,0));
    return angleAndaxis.angle()*angleAndaxis.axis(); 
}

DataCollectorT::TransVec DataCollectorT::GetTransVec(const Suit & suit)const 
{
    const Eigen::Matrix4d & T = suit.second;
    Eigen::Vector3d transvec = T.block<3,1>(0,3);
    return transvec;
}


_E_DATA_COLLECTOR_