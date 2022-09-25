<!--
 * @Author: Liu Weilong
 * @Date: 2021-02-04 09:45:10
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-04-26 17:51:35
 * @FilePath: /Codes/31. orb_slam_related/YGZ/doc/code_reading/feature_manager.md
 * @Description: 
-->
### Cache机制
1. 添加机制
a.1. 在Cache 内部进行搜索，找不到足够多的点的时候,进行原版本ORB 的SearchLocalMap<br>
     然后把追踪到的点放入Cache当中。<br>
a.2. 在生成KeyFrame的时候，把所有新生成的MapPoint 加入到Cache<br>
2. 删除机制
a.1. 在TrackLocalMapDirect 里面， 如果发现跟踪不上了，就进行删除.但是这个一定程度上会影响到LocalMapping 内部 依靠 Found 和 Visible 进行判断的函数
2021.2.22 补充<br>
最后发现YGZ只是添加了 Found,Visible添加并没有管如何使用，但是最终Mapping结果并没有收到影响，所以猜测，是因为MapPointCulling 里面的冗余设计起了效果。
3. 特征提取机制
以下 CurrentFrame.N！=0 也就是有跟踪点的Frame 叫Clear
    CurrentFrame.N ==0 完全没有跟踪点的Frame 叫PureClear 
a.1. StereoInitial 对 PureClear 进行ORB双目提取 让PureClear  变为Clear
a.2. TrackSparse 属于 对 PureClear 进行灰度误差项的优化 
a.3. TrackWithMotionModel 属于 对 PureClear 提取 双目ORB 让PureClear  变为Clear 然后使用ORB 原本的Track
a.4. TrackReferenceKeyFrame 属于 对 PureClear 提取 双目ORB 让PureClear  变为Clear 然后使用ORB 原本的Track
a.5. TrackLocalMapDirect 分三个阶段  
     前一个阶段：Align2D 的过程 会产生匹配的过程 但是没有调用 让PureClear  变为Clear
     后一个阶段：(options) TrackLocalMap 进一步加大N
     最后阶段进行优化
a.6. TrackLocalMap  对 PureClear 提取 双目ORB 让PureClear  变为Clear 然后使用ORB 原本的Track
a.7. CreateNewKeyFrame 如果之前是一路 直接法顺下来没有任何 ORB track 的调用 就会使用 左目DSO提取 右目ORB 的提取方式

特征提取机制需要后续进行测试

### 点关系的维护
1. StereoInit之后，生成第一波MapPoint
2. 这个时候，MapPoint 数量不足，一直跑ORB的代码。不断进行TrackWithMotionModel 来建立点直接的联系
3. 跑了一段时间之后，MapPoint 数量充足，SparseImgAlign不建立关系，TrackLocalMapDirect 建立点之间的关系。
4. 如果TrackLocalMapDirect中 SearchLocalMapDirect 点不足，就使用LocalUpdate 中的UpdateLocalMap 添加点，但是建立关系还是在
   TrackLocalMapPDirect里面的光流进行
5. LocalMapping，后端会存在进一步的关系建立















