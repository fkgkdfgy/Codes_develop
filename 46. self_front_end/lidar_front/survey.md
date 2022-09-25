<!--
 * @Author: Liu Weilong
 * @Date: 2021-06-10 11:47:09
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-10 15:32:34
 * @Description: 
 * 调研笔记
-->
### Sketch
序号|文章名|分类|备注
---|----|---|---
1. |MULLS: Versatile LiDAR SLAM via Multi-metric Linear Least Square||


-------

1. MULLS:Versatile LiDAR SLAM via Multi-metric Linear Least Square
   
   related work:

   local registration:

   classical ICP 为了处理Dense Point Cloud 2 Dense Point Cloud
   variant ICP   更改 sample point for matching 和 error metric [48]-[51]
   LOAM          选择 边和面特征 + 分类点到特征的方式进行ICP优化
   LeGO-LOAM     进一步对点云进行分割
   SuMa SuMa++   match between range image and surfel map
   point 2 plane [52]-[54]

   global registration:
   
   [31]-[35]     使用global feature 估计方位角?
   [55][56][30]  直接求解6Dof 也是特征?

   摘要:
   
   joint optimzation with point2line pint2point point2plane 

   global registration with TEASER++

   内容:

   A. 去畸变: SLEPP进行插值
   B. 特征提取:
      b.1. 去地面点
      b.2. PCA 计算特征: K-R 搜索最近邻, PCA 分析出  Vertex Pillar Beam Fascade Roof 五种特征
      b.3. Encode 特征: 通过特征含量比例、Intensity、高度 进行 encode 用于之后计算。
   C. 多种特征混合 ICP:
      c.1. 各类别的Kdtree搜索: 类别搜索、distance check、direction check
      c.2. 混合策略的weight 策略: resiudal(M-esitmator)\Category\Intensity
   D. BackEnd : 使用NCC 和Teaser ++(进行粗估计) + MULLS ICP 进行精确估计






