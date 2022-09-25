<!--
 * @Author: Liu Weilong
 * @Date: 2021-02-23 09:30:56
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-23 14:13:27
 * @FilePath: /3rd-test-learning/32. vins_related/origin/code_reading/doc_sketch.md
 * @Description: 
-->
### 代码框架整理内部
1. 只看理论和实现方式，而不是代码

### 框架

序号|模块名|对应doc|状态
---|---|----|----
1. |特征提取|                vins_feature_tracker.md|完成
2. |Odometry Matching|      ORB_tracking.md  + ORB_extract_and_match.md|完成
3. |特征管理  |              ORB的特征管理分散在 Tracking 和 Mapping当中，有些混乱|完成
4. |Mapping Matching|       ORB_mapping.md + ORB_extract_and_match.md|完成
5. |地图维护|                ORB2 并没有这个部分
6. |回环检测|                ORB_DBoW.md + ORB_loopClosing.md(部分完成)<br>|部分完成
7. |工具类|                  |还没有进行梳理
8. |Supplement Material (基础补全)| supplement_material.md<br>

### remain theory problem
序号|名字|状态
---|---|----
1. |EPnP|           完成
2. |Optimizer|      完成
3. |g2o边缘化实现|
4. |双目的特征提取 |   完成

### 总结
1. 为什么说VINS-Mono 太薄了前端?
   只有 F2F 没有 F2Map