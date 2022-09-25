/*
 * @Author: Liu Weilong
 * @Date: 2021-05-22 22:34:25
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-28 06:54:18
 * @Description:
 * 1. 角度递推基本没有问题
 * 2. 位置递推有问题 (递推存在问题? 或者是因为数据集的原因？) 目前的结论: 递推绝对有问题 递推正确了？？我的天？？怎么就对了？最后发现是频率的问题
 * 3. Error State 递推和方差递推 TODO
 */
#include "new_eskf.h"

int main()
{
    DataLoader dl("../all_paths.yml");
    DataLoader::ElementInfo info;
    DataLoader::ElementInfo oldest_info;
    
    dl.GetInfo(oldest_info);
    dl.Reset();
    
    ESKF eskf;

    int count = 0;
    int count_th = 6000;

    while(dl.GetInfo(info))
    {
        eskf.PushIMUInfo(info);
        count++;
        if(count == count_th) break;
    }

    auto state = eskf.GetState();
    auto ref_state = eskf.GetRefState();
    
    // 对比 state 和 ref_state
    cout<<"----- State-----"<<endl;
    cout<<"State Position :"<<state.posi.transpose()<<endl;
    cout<<"State Attitude :"<<state.theta.transpose()<<endl;
    cout<<"----- Ref State-----"<<endl;
    cout<<"Ref State Position :"<<ref_state.posi.transpose()<<endl;
    cout<<"Ref State Attitude :"<<ref_state.theta.transpose()<<endl;    
    cout<<"Ref Vel Explorate Posi:"<< eskf.ref_vel_explorate_posi_.transpose()<<endl;
    // 对比真值
    Sophus::SO3d rotation_SO3(Eigen::Quaterniond(info.atti(0),info.atti(1),info.atti(2),info.atti(3)));

    cout<<"the real position :"<< (info.posi - oldest_info.posi).transpose()<<endl;
    cout<<"the real attitude :"<< rotation_SO3.log().transpose()<<endl;
    
    return 0;
}