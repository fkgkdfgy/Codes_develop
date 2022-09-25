<!--
 * @Author: Liu Weilong
 * @Date: 2021-04-19 10:16:14
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-04-19 10:24:57
 * @FilePath: /Codes/38. line_feature/plucker/pl_slam_point_opt.md
 * @Description: 
-->
### PL-SLAM 两点参数化
1. 参数化表示
   使用两端点参数化
2. 误差项含义点到观测直线的
   点到直线距离
3. 优化过程
   $$
    r = \left[
        \begin{matrix}
            s^Tn\\
            e^Tn
        \end{matrix}
        \right]
        \\
    \left[
        \begin{matrix}
            s\\
            e
        \end{matrix}
    \right] =     \left[
        \begin{matrix}
            \pi(KTp_s)\\
            \pi(KTp_e)
        \end{matrix}
    \right]
   $$