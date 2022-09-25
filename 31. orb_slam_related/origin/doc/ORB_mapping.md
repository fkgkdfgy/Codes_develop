<!--
 * @Author: Liu Weilong
 * @Date: 2021-01-28 15:08:16
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-04-21 14:35:01
 * @FilePath: /Codes/31. orb_slam_related/origin/doc/ORB_mapping.md
 * @Description: 
-->
### ORB LocalMapping
内容来自LocalMapping.cc LocalMapping.h
1. ProcessNewKeyFrame
2. MapPointCulling
3. CreateNewMapPoints
4. SearchInNeighbors
5. KeyFrameCulling
   
------
没有什么理论

------
1. ProcessNewKeyFrame\
   更新平均NormalVector、Depth、Discriptor，更新共视关系\
   把keyFrame 添加到Map 中\
   对于新生成的点添加到mlpRecentAddedMapPoints 留待之后进行检验\
2. MapPointCulling (可以更换成收敛的方式)\
   更新mlpRecentAddedMapPoints 满足以下条件被直接去掉\
   foundRatio<0.25 || 生成FrameId 和目前Current 相差>=2 且 被观测次数< cnTH || 生成FrameId 和目前Current 相差>=3\
   (注：foundRatio  mnFound 来自于每一次TrackLocalMap PoseOptimization 优化之后 表示确实看到了\
                   mnVisible 来自于每一次SearchLocalPoints 在inFrustum 判断之后 表示应该能看到)\
                   
3. CreateNewMapPoints 在SearchForTriangulation的注释中得以看到这个函数存在的意义是为了生成一些没有跟踪上的MapPoint 没跟踪上的原因有很多\
   有了一些新的思考，这里的确是Create 新的地图点，将一些之前没有跟踪上的，有高视差的点进行三角化,\
   但是这个地方的确会重复创建地图点\
   <font color="Red"> 这里就体现了，跟踪需要连续，三角化缺需要大视差的矛盾</font>
3.a. 得到mpCurrentKeyFrame 高共视的关键帧\
3.b. 对每一个关键帧\
     检测中位数深度/baseline 的ratio check1\
     生成Fundamenal Matrix F12\
     SearchForTriangulation 寻找CurrentFrame 和 关键帧之间的匹配\
     对每一个匹配点\
        检查视差(检查新两帧的视差和Stereo视差的大小，选择其中大的视差进行后面的三角化) check2\
        三角化\
        检测地图点到两幅图像的重投影误差    卡方分布 check3 \
        深度比例和金字塔层数的check             check4\
        生成新的地图点(加观测、深度、NormalVector更新、加入mlpRecentAddedMapPoints)\

4.  SearchInNeighbors (可以后面进行更换)\
4.a. 得到mpCurrentKeyFrame 高共视的关键帧，和这些关键帧的高共视关键帧\
4.b. 对每一个目标关键帧 Fuse mpCurrentKeyFrame的地图点\
4.c. 得到所有目标关键帧的 地图点\
4.d. Fuse   mpCurrentKeyFrame 和 4.c. 得到的地图点\
需要SearchInNerighbors 的原因是CreateNewMapPoints 出现了太多 同一个3D点 对因多个MapPoint的情况

<font color="Red">2021.4.21添加 <br></font>
在现在有了累计误差是和更古老的帧匹配进行消除这一个概念之后，我们再来看一整套LocalMapping 的算法，就会发现，的确里面充满这方面思想的实现。





1. KeyFrameCulling 清理多余KeyFrame
   