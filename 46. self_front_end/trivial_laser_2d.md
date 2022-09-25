<!--
 * @Author: Liu Weilong
 * @Date: 2021-06-23 11:46:22
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-23 11:55:32
 * @Description: 
-->

### 2D laser LIO设计
 
 去畸变等
1. 最前端 loam feature extraction
2. 前端ndt 地图维护 + 栅格内 feature 精细化分类 + EM 算法寻找outlier
3. 前端 intensity 滤波
4. 后端LocalMap 仿LOAM 设计 九格localmap
5. 最终gtsam 维护全部优化关系