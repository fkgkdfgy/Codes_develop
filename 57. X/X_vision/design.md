<!--
 * @Author: Liu Weilong
 * @Date: 2021-06-29 20:05:55
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-30 13:05:48
 * @Description: 
-->

### BASALT 原理部分的填充
1. nullspace 检测
2. NFR
3. 后端组成逻辑



### 目标--深度修改basalt
1. 单线程化
2. 更换VINS初始化
3. BASALT SE2+循环检验
4. 添加SVO 的localMap 跟踪扩大前端匹配
5. 添加特征描述子 module 方便用于重定位
6. 后端进行SVO概率分布管理
7. 添加 光度标定 options
8. BASALT 点管理
9.  后端进行描述子的帧间关联(ORB)
10. 后端更换成ICE-BA
11. BASALT的SO3预积分
12. 重定位添加
13. PoseGraph 使用NFR 加速计算
14. 后端ColMAP进行建图
15. 定位模块
16. IMU辅助的各种RANSAC

### 目标--better BASALT
1. 添加语义信息
2. ORB的根据描述子的重复特征融合
3. imu 辅助的动态环境克服
4. imu 辅助实时去除图像运动模糊(motion-blur) + 点的时间戳调整
5. 多视角融合+Gazebo仿真(做完准备上kitti)
6. 前端进一步对畸变模型进行描述
7. 多线程适配
8. 添加特征描述子的定位环节
9. 添加多种描述子的module
10. 深度学习粗定位
11. 精定位

===