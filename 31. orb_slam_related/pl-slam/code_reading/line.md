<!--
 * @Author: Liu Weilong
 * @Date: 2021-03-15 13:06:24
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-24 13:50:19
 * @FilePath: /3rd-test-learning/31. orb_slam_related/pl-slam/code_reading/line.md
 * @Description: 
-->
### 内容只有PLSLAM 线的部分
1. detectStereoLineSegments
2. matchKF2KFLines
3. MapLine 
4. OverLap
5. removeOutliers


-------------

### detectStereoLineSegments

序号|子任务|内容
----|----|----
1. |detectLineFeatures|LSD 版本 使用response 进行排序,取高响应<br>FLD 版本 使用长度进行排序，取长的<br> 最后算一下描述子
2. |matchStereoLines|a. GridMatch 加速匹配方法 暂用match(opencv) 代替<br>b.lineSegmentOverlapStereo overlap的一个check<br>c. filterLineSegmentDisparity 视差的check


### matchKF2KFLines
序号|子任务|内容
---|---|---
1. |matchGrid|一个Grid小窗口加速
2. |match|左右匹配、内部调用matchNNR
3. |matchNNR|内部调用KnnMatch 取前两个，如果第一个没有比第二个小nnr倍就被抛弃


### MapLine
MapLine、MapPoint只在后端进行创建，使用full_graph 进行共视关系的维护

### OverLap
序号|名字|内容
---|---|----
1. |lineSegmentOverlapStereo|双目overlap 对左右线段进行v(pixel)上的重叠确认 对于垂直线匹配不适用
2. |lineSegmentOverlap|

### removeOutliers
直接使用Outliers 组成的一维高斯分布进行去outliners