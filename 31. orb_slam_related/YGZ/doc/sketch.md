<!--
 * @Author: Liu Weilong
 * @Date: 2021-02-04 13:33:28
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-16 11:31:50
 * @FilePath: /3rd-test-learning/31. orb_slam_related/YGZ/doc/sketch.md
 * @Description: 
-->
### YGZ 对于 ORB 的改善
1. Cache的机制，加快TrackLocalMap搜索过程
2. SingleLayerDSO 的提取机制 已经知道了
3. InverseCompositional 直接法 用于帧间匹配
4. TrackLocalMapDirect 图块的Affine匹配 和原版本的
5. 所有的MapPoint确保都是从双目算出来的，不用担心尺度的问题
6. 特征提取的部分还没有看懂 已经看懂


### 能够加速的原因
1. Tracking 部分\
   |___ ComputeStereo的次数降低    大项\
   |___ fast 使用 sse加速 且单张图 nLevel=0 \  
   |___ DBoW 计算次数减少 dso 的时候已经单层描述子       有更多的时候基本不计算特征    大项\
   |___ TrackLocalMap 的检索时间减少 大项\
   |___ Inverse Compositional  的方法\

### 关于