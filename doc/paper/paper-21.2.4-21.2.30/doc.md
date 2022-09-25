<!--
 * @Author: Liu Weilong
 * @Date: 2021-02-04 15:39:23
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-02-26 09:41:33
 * @FilePath: /3rd-test-learning/doc/paper/paper-21.2.4-21.2.30/doc.md
 * @Description: 
-->
### SVO 再探
1. 匹配过程
   SparseImageAligned
   PyrDirectMethod 到三层左右停下来 进一步加速+鲁棒核 
   TrackLocalMap 抗光照模型也就是那个 -m 
   |___ Patch Affine + 图层判断
   |___ Edgelet 
2. Mapping
   Depth Filter
3. Motion Piror
   直接当作优化项写在SparseImageAligned里面

### Accurate and Robust Scale Recovery for Monocular Visual Odometry
摘要： 用地面点进行坐标scale 恢复<br>
内容：<br>
1. 提取单帧地面点
2. 汇集多帧地面点
3. 进行综合的优化
4. 用 地面点建模的plane 生成一个ratio 进行scale恢复


### Leveraging Planar Regularities for Point Line Visual-Inertial Odometry



### Online Temporal Calibration of Camera and IMU using Nonlinear Optimization
在进行IMU误差分析的时候，将$\DeltaT$引入进行误差分析

### Online Temporal Calibration for Monocular Visual-Inertial Systems
利用匀速假设在优化的时候调整 p2d(u,v) 在像素平面上的位置。最终优化得到td   <br>
优化公式
$$
   e = p_{2d(u,v)} - \pi{p_{3d(x,y,z)}}
$$

