/*
 * @Author: Liu Weilong
 * @Date: 2021-01-04 09:22:18
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-04 11:14:33
 * @FilePath: /3rd-test-learning/11. utils/data_collector/data_collector_interface.h
 * @Description: 
 */
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define _S_DATA_COLLECTOR_ namespace data_collector{
#define _E_DATA_COLLECTOR_ };

_S_DATA_COLLECTOR_

template<typename T>
class  DataCollectorInterface
{
    public:
    using Type = T;

    DataCollectorInterface(const string & store_path, const string & info_name)
    :store_path_(store_path),info_name_(info_name)
    {
        fout_.open(store_path_.c_str(),std::ios_base::trunc);
        if(!fout_.is_open())
        {
            std::cerr<<"the file openning meets some problems!!"<<endl;
            abort();
        }
    }

    virtual void StoreInfo() =0; 
    virtual bool push_back(const T & input_info) =0;

    virtual ~DataCollectorInterface(){};

    const string store_path_;
    const string info_name_;
    std::ofstream fout_;
    
};





_E_DATA_COLLECTOR_