<!--
 * @Author: Liu Weilong
 * @Date: 2021-05-04 09:24:25
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-05-07 11:00:35
 * @Description: 
-->

### ICE-BA 主要流程
主要内容来自 Euroc_test.cpp
1. 得到照片
2. 得到标定参数
3. 计算视场角和mask  mask就是说在去畸变后，哪些pixel 是在视场角内，哪些不在，不在视场角内的数值为Oxff 在的为Ox00
4. 加载imu数据


----
各种标号看蒙了
输入: keypoint 的观测
输出: CF KF 

信息流打通拉!! 哈哈哈哈

ixsSort   


1. create_iba_frame
   
2. PushCurrentFrame -> PushCurrentFrame() 把初始的CurrentFrame数据 ， convert to InputLocalFrame m_ILF
    |__ ConvertCamera  转换相机坐标
    |__ ConvertDepth   转换深度 和 尺度
    |__ ConvertFeatureMeasurements 转换测量特征
        |__ 第一帧数据没有放任何东西进 m_zsSortTmp
    |__ 保存imu数据   保存在 m_ILF.m_us 内部  并且进行了去掉bias的操作
3. PushCurrentFrame -> PushKeyFrame()  把外部KF 数据转换成 内部使用的KF 数据  
    |__ 转换相机位姿和状态(位姿、速度、两个bias)
    |__ ConvertDepth   转换深度和尺度
    |__ ConvertFeatureMeasurements 转换测量特征
    |__ 保存 KF 位姿到 m_CsKF
4. PushCurrentFrame -> m_LM ->IBA_PushLocalFrame  把m_ILF 保存到 m_LM->m_CsLF
5. PushCurrentFrame -> m_LM ->IBA_PushKeyFrame 
    |__ 把m_IKF  保存到 m_LM->m_CsKF
    |__ 


