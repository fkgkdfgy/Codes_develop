<!--
 * @Author: Liu Weilong
 * @Date: 2021-05-10 13:22:03
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-24 13:02:47
 * @FilePath: /Codes/47. basalt/code_reading/optimize.md
 * @Description: 
-->
### 优化部分的问题
1. 入口 measure
2. 信息管理逻辑
3. 重要成员函数解析
4. 优化的相对量的逻辑


-----

KeyPoint_vo

1. 信息管理逻辑  measure 一起进行分析
    |__ meassure 输入
        |__ opt_flow_meas                   光流跟踪结果
        |__ meas                            IMU积分数据 已经进行过预积分
    |__ 信息管理
        |__ frame_state                     map CameraTimeId state 存储recent的 camera state ????什么是recent 这里存疑
        |__ frame_poses                     存疑??? 存储old_keyframes??
        |__ imu_meas                        保存所有的imu 预积分量
        |__ prev_opt_flow_res               保存所有的光流跟踪结果
        |__ lmdb                            LandMarkDataBase 用于特征管理
            |__ kpts                        map id KeyPointPostion 用于保存三维点
            |__ obs                         host frame 对应的 target frame 上的观测
            |__ kpts_num_obs                map id int 用于保存keypoint 被观测到的次数
            |__ host_to_kpts                保存 keyframe 对应的 kpts id
        |__ kf_ids                          存储所有keyframe 的id
        |__ 
    |__ 算法流程
        |__ for each_camera                 统计三维点观测和非三维点的观测，作为生成关键帧的判断
            |__ for each_obs
                |__ check connected  
        |__ NeedNewKeyFrame
        |__ CreateNewKeyFrame
            |__ for each obs in unconnected_obs0
                |__ for each opt_flow_res in prev_opt_flow_res
                    |__ for each camera
                        |__ FindSameLandMarkId        所有找到对应关系的点，都会被保存在std::map< TimeCamId, KeypointObservation> kp_obs 保存 对应帧的id 这个点的id 和 这个点在那个帧的上 像素观测
        |__ triangulate                      三角化之后，保存host frame id 和lm_id 到lmdb
        |__ optimize
            |__ checkOPTcondition
            |__ Fill AbsOrderMap             填充状态变量管理器 这里也就是 填充 所有 keyframe  pose frame 的Size 填充frame_state内 pose vel bias的Size
            |__ for iter< vio_max_iterations
                |__ linearizeHelper          这里主要是构建 视觉的各个 jacobiian 和hessian 小块    
                |__ 


        |__ marginalize

2. 重要成员函数解析 -- linearizeHelper
    |__ 输入 
        |__ lmdb.obs       host frame 对应的 target frame 上的观测
    |__ 输出
        |__ rld_vec        相对位姿和landmark的hessian block 存储顺序对应 lmdb.obs 这个map 的存储顺序
        |__ error
    |__ 重要的流程变量
        |__ RelLinData     rld  每一个host frame 对应的hessian block
            |__ order           map < host_frame_id, target_frame_id> 用于保存 内部 vector jacobian 小块的索引序号 i.e. order[ 1] = {11,23} 也就是说 d_rel_dh[ 1] d_rel_dtp[ 1] 保存着 host frame11 和 target frame 23的 jacobian block 
            |__ d_rel_d_h       relative_pose / host_frame_pose 的jacobian
            |__ d_rel_d_t       relative_pose / target_frame_pose 的jacobian
            |__ Hll             map < KeyPointId,Matrix3d> 保存landmark id 对应的hessian 小块
            |__ bl              map < KeyPointId,Vec3d>保存landmark id 对应的b 小块
            |__ error           存储residual
        |__ FrameRelLinData frld 保存视觉residual相关的jacobian 和 hessian 小块
            |__ lm_id           保存kpt 的id，序号直接对应Hpl  i.e. [ 1] = 11
                                Hpl[ 1] 就是kpt 11 和 relative pose 的对应hessian
            |__ Hpl             就是kpt  和 relative pose 的对应hessian
            |__ bp              pose 的 b
            |__ Hpp             relative pose 和 relative pose 的hessian
    |__ 算法流程
        |__ for each obs create hessian block  这里也就是例遍 lmdb.obs 进行block 构建 但是这里还没有进行填充
        |__ for each obs in lmbd.obs (for host in lmdb.obs)
            |__ for each < target_frame_id,point> 
                |__ if(host_frame_id != target_frame_id) 
                    |__ calculate RelativePose Jacobian    存储在 rld
                    |__ calculate Landmark Hessian         计算这个target frame 下面所有的观测对应的 hessian 存储在 frld ，最终frld 存放进rld
                |__ if(host_frame_id == target_frame_id) 
                    |__ calculate Landmark Hessian



   



















4. 优化的相对量的逻辑(TODO)