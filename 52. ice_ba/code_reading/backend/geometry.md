<!--
 * @Author: Liu Weilong
 * @Date: 2021-05-07 06:37:18
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-07 07:42:55
 * @Description:  
-->
1. Camera
    |__ class Calibration
        |__ Intrinsic          m_K          相机内参
        |__ Rotation3D         m_Ru         相机外参旋转   左相机到imu
        |__ Point3D            m_pu         相机外参平移
        |__ AlignedVector3f    m_ba,m_bw    imu 的两个bias
        |__ (Stereo Options)
            |__ Intrinsic      m_Kr         右相机内参
            |__ Rotation3D     m_Rr         相机外参旋转   右相机到imu
            |__ Point3D        m_pr         相机外参平移
    |__ class Pose
        |__ Quaternion         m_q          四元数
        |__ Point3D            m_p          位移
    |__ class Factor
        |__ class Unitary      对角线部分和增广的那一行的b Camera_i-Camera_i
            |__ class CC       
                |__ Matrix6x6f m_A        在H 矩阵中 Camera-Camera 位姿的 6×6 的小H矩阵
                |__ Vector6f   m_b        b中 Pose 对应的部分b
            |__ class CM                  Camera(T)-Camera(v,bias_a,bias_w) 6×9 的小H矩阵
            |__ class MM  
                |__ Matrix9x9f m_A        在H 矩阵中 Motion-Motion 位姿的 9×9 的小H矩阵
                |__ Vector9f   m_b        b中 Pose 对应的部分b
            |__ CM m_Acm                  H的cm这部分 c就是相机pose
            |__ MM m_Amm                  H的mm部分,-b的m部分 m就是运动部分
        |__ class Binary       非对角线数据 Camera_i-Camera_j   前一帧和后一帧
            |__ CC m_Acc
            |__ CM m_Acm
            |__ MC m_Amc
            |__ MM m_Amm
        |__ Unitary m_Au
        |__ Binary  m_Ab
