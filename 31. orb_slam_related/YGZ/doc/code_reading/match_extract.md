<!--
 * @Author: Liu Weilong
 * @Date: 2021-02-02 17:22:53
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-04-25 20:34:45
 * @FilePath: /3rd-test-learning/31. orb_slam_related/YGZ/doc/code_reading/match_extract.md
 * @Description主要
-->

### SingleLayerDSO提取

只要提取特征点，就是把整幅图，按格子划分，每个格子最多FAST三个点\
GFTT 的response\
如果点数过多，就调整格子大小，再来一次


### SparseImageAlign
利用五层金字塔图像，进行从粗到细的InverseCompositional 直接法\
但是这里只是算出 mCurrentFrame 的位置，并没有对CurrentFrame 的Key 和 MapPoint 进行填充。\
使用的投影点来自LastKeyFrame 的MapPoint


### InverseCompositional 直接法
用于帧间匹配
1. precomputeReferencePatches
2. computeResidual

------

Inverse Compositional LK 原理见 Supplement Material\
双线性插值：https://zh.wikipedia.org/wiki/%E5%8F%8C%E7%BA%BF%E6%80%A7%E6%8F%92%E5%80%BC\


------
1.a. precomputeReferencePatches 用于提前计算Jacobian\
a.1. 对每一个点进行\
       计算出优化变换的Jacobian($J^{u,v}_{\xi}$)求导(在template内部多次使用)\
       双线性插值出每一个template 的数值\
       计算出每一个template的综合Jacobian($J^I_{u,v}J^{u,v}_{\xi}$)\
2.a. computeResiduals
这部分具体内容见 Theory.md

### 块匹配
内容主要来自FindDirectProjection() 用于ref 块在cur上找到最佳的匹配\

a. GetWarpAffineMatrix() 得到cur和ref 在点周围的Affine\
ref_point在ref上面,向左5个格子取一个像素，向下五个格子取一个像素，通过相对运动和外参找到cur上面的对应的像素。构建一个affine
b. GetBestSearchLevel() 计算合适的图像金字塔层数\
   这里依靠Affine的determiant 判断，也就是那个平行四边形的面积，来判断是不是找到了合适的层数
c. WarpAffine() 对p_ref 周围的template 进行反向包装\
也就是把p_ref 一个平行四边形的template 通过Affine 变成 正方形，用于和cur 图像上的匹配\
d. 去掉边缘的点
e. Align2D()用ref的方块在cur 寻找合适的匹配\

收敛了就说明匹配上了，但是感觉不怎么靠谱，应该没有别的方法\
这里的具体理论看theory.md

### 关于特征的提取
1. 有了一个变化，左目没有再提取ORB特征，右目继续使用ORB特征，这里的匹配竟然能匹配上也是很神奇。

### TrackLocalMapDirect 
1. SearchLocalPointsDirect 寻找 子图在CurrentFrame 上面的匹配关系，然后填在CurrentFrame 的mvKey 和mvMapPoint 上面。<br>
a.1. 先从Cache 内部，进行历遍，如果点不够，走一次SearchLocalMap,填充LocalMap点\
a.2. 对于每一个MapPoint
        对于这个MapPoint 的每一个观测做一次 块匹配
        统计在CurrentFrame 上面的投影平均值
        填充CurrentFrame的 Key 和 MapPoint
a.3. PoseOptimization

### 关于特征点之间的联系的建立
因为主要是光流的原因，所以在SparseImageAlign 和 Align2D 的时候，其实并没有特征点关联。
特征的关联应该主要是在后端，和前端光流clahe不足进行 TrackLocalMap 的时候。
        