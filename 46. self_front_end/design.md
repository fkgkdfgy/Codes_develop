<!--
 * @Author: Liu Weilong
 * @Date: 2021-05-07 14:27:26
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-24 11:27:52
 * @FilePath: /Codes/46. self_front_end/design.md
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
6. 前端进一步对畸变模型进行描述
7. 后端进行SVO概率分布管理
8. 添加 光度标定 options
9. 后端进行描述子的帧间关联(ORB)
10. 后端更换成ICE-BA
11. BASALT的SO3预积分
12. 重定位添加
13. imu 辅助的动态环境克服
14. 添加特征描述子的定位环节
15. 添加多种描述子的module
16. PoseGraph 使用NFR 加速计算

### 目标--better BASALT
1. 添加语义信息