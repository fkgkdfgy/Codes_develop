<!--
 * @Author: Liu Weilong
 * @Date: 2021-04-23 09:24:09
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-04-27 10:36:20
 * @FilePath: /Codes/34. svo/svo_edgelet/doc/epi_search.md
 * @Description: 
-->
### 极线搜索的主要内容
1. updateSeeds
2. findEpipolarMatchDirect
3. 仿射详解
4. addFrame 
-----

1. updateSeeds
   1.a while(seed_iter != seeds.end())
      a.1  进行坐标转换，查看是不是在当前Frame 内
      a.2  通过seed的 $\mu$ 和 $\sigma$ 判断最大和最小的距离
      a.3  findEpipolarMatchDirect 极线搜索
      a.4  进行seed 更新

2. findEpipolarMatchDirect
   2.a 由最大、最小距离计算归一化平面上的点坐标
   2.b 计算仿射、最优匹配层、得到仿射
   2.c 如果搜索范围小直接仿射光流得到亚精度匹配结果，然后进行三角化
   2.d 搜索范围大，ZNCC 过一遍，然后在最优匹配附近进行仿射光流匹配，得到亚精度结果，进行三角化
   
3. 仿射详解
   为什么需要仿射？ 克服尺度和SE3的运动
   WarpAffine() 如何求解仿射
   i.e. 
   将像素的 patch 看成一个平面
   在ref 上面求解 (0,4) 和 (3,0) 给上一个深度之后，
   在cur 上面对应的px 可能是 (-3,1) 和 (2,2)那么仿射矩阵就是
   $$
    ACR = \left[
        \begin{matrix}
            2/4&-3/3\\
            2/4&1/3
        \end{matrix}
        \right]
   $$

4. addFrame 
   while(seeds) 进行极线匹配，然后更新seeds (tips:进行仿射搜索)