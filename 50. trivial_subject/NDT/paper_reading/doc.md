<!--
 * @Author: Liu Weilong
 * @Date: 2021-06-02 08:58:15
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-02 15:42:54
 * @Description: 
-->
1.  Probabilistic Normal Distributions Transform Representation
for Accurate 3D Point Cloud Registration

    NDT 存在的问题:
    a. cell size 难以选取的问题 -> 所以出现多层的NDT[5][6]
    b. cell size 导致出现分布分割计算的问题 -> 所以出现内部先聚类的NDT [7][8]
    c. cell 导致的不连续性  插值解决 [1][9]
    d. 速度的问题采用 d2d_NDT[4]

    摘要: 目的是把一些没有形成分布的cell点用起来

    主要内容:
    
    对每一个点进行概率建模,防止出现算法上的特征值退化的问题？ 感觉没有解决掉什么比较大的问题。 退化的问题能够让代码更好吗？

    测试的时候使用d2d——NDT 进行验证，效果显示的确还是可以的

    使用八叉树进行分层

2. Voxelized GICP for Fast and Accurate
3D Point Cloud Registration

    GICP: distribution to distribution [8]
    NDT: 使用网格进行加速
    GICP和ICP 速度慢 KD-tree 卡脖子
    NDT: 需要好好确认voxel 大小 