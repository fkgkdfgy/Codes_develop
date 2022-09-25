<!--
 * @Author: Liu Weilong
 * @Date: 2021-03-31 15:37:19
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-04-07 07:07:50
 * @FilePath: /3rd-test-learning/doc/slam_back_end/paper_reading.md
 * @Description: 
-->


-----
优化四步
1. 构建误差项
2. 进行Schur补
3. 求姿态
4. 求点的位置
-----




1. Nonlinear Graph Sparsification for SLAM:
   相关阅读: 利用 sparse nature 11 15 17 
   当多次，边缘化之后信息矩阵变得dense，需要使用sparse enforce method 16 3 23 13 
   Margine 过程基于高斯假设，会引入误差，因为次优的线性化点

   SEIF 21 -> 8 -> 23 They also noted that the
    optimization need only be carried out on the Markov blanket of
    the node to marginalize
    马尔科夫毯

   14 16 用信息矩阵决定 margin 掉哪一个-> 

   摘要：
   a. 通过找一些set mean 和 covariance 来最小化KL散度，以此来对问题进行近似
   b. 好处: b.1. 最小化问题是凸的，也就是说有最优解 b.2. 框架对于所有nonlinear 函数通用 b.3. 保留矩阵的块结构
           b.4. 在一些情况下 存在解析解

   <font color ="Red">这种看不懂的东西，就是需要摆正心态去看，而不是急于求成。</font>

   内容:
   a. 为了解决什么问题?
   还是为了解决margin 之后稠密,用一个Sparse 的分布进行近似，而且错误的FEJ的问题
   b. 整体框架是什么?
   杂记：KLD散度进行拟合
   在SLAM 框架下 如何应用
   b.1. 提取马尔可夫毯
   b.2. 使用马尔可夫毯内的measurement 进行一次初始化
   b.3. 使用Schur补进行margin
   b.4. 选择虚拟measuremnt 来进行模拟 -> 论文的内容 (三种拓扑结构)
   b.5. 放回

2. Fast Nonlinear Approximation of Pose Graph Node Marginalization
   笔记:
   
   $$

   $$

3. Monocular Graph SLAM with Complexity Reduction
   