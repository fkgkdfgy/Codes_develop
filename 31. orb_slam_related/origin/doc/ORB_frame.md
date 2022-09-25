<!--
 * @Author: Liu Weilong
 * @Date: 2021-01-30 19:18:55
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-02-01 10:20:10
 * @Description: 
-->
### ORB Frame
主要内容，来自双目生成Frame.cc Frame.h 的 Stereo 生成
1. ctor
2. ComputeStereoMatches
3. isInFrustum

---------
SAD 匹配算法用于ORB 双目匹配的亚精度匹配
具体见Supplement Material

----------
1.a. Ctor -> 提取特征、去畸变、双目匹配、配置格子分别放置

2.a. ComputeStereoMatches 双目匹配\
a.1. 统计右目特征，并按行标号存储\
a.2. 左目特征按行进行搜索，描述子匹配符合条件的特征进行SAD\
a.3. 取SAD最优匹配附近的三个SAD匹配,三个点做一个抛物线拟合找到最优，进行亚像素的修正\

3.a. isInFrustum \
     检查Frustum 的意思是\
     视觉点在空间中，存在一个大概的视角，按道理说，如果视角相差过大，他们不应该会存在一个共视关系\
     所以这里，就是来检查，这个MapPoint普遍的观测角，和当前的观测是不是视角差距过大\