<!--
 * @Author: Liu Weilong
 * @Date: 2021-03-18 09:55:48
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-18 14:22:01
 * @FilePath: /3rd-test-learning/31. orb_slam_related/origin/doc/ORB_Mono.md
 * @Description: 
-->
### ORB 单目的内容
1. 初始化
2. 匹配

----------

### 初始化
序号|子步骤|备注
----|---|----
1. |ExtractFirstFramePoint|从第一帧提取出关键点
2. |Explicit Initialization|第二帧来的时候，进行<br>a. SearchForInitialization 得到两帧之间的匹配特征点<br>b. findHAdndF 使用找HF哪个合适<br> c. 从HorF当中获取合适的Rt(F 四种/H 八种) 然后进行3D点的恢复,选择合适的Rt
3. |CopyRtToCurrentFrame|
4. |CreateInitialMapMonocular|a. 创建关键帧<br> b. 添加地图点 <br> c. 做一次GBA

