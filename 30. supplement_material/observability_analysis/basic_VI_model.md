<!--
 * @Author: Liu Weilong
 * @Date: 2021-04-22 09:20:47
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-04-22 13:35:39
 * @FilePath: /Codes/30. supplement_material/observability_analysis/basic_VI_model.md
 * @Description: 
-->
### IMU state 递推
这里应该是分连续和离散两个部分进行。
a. 离散方便计算，理解容易，但是因为线性化的问题，会引入不必要的误差，导致系统的可观性发生变化。
b. 连续理解起来比较难，但是会更加精确，让系统引入尽可能少的误差

所以这里我们使用连续模型来对IMU 预积分模型进行递推

### IMU 连续递推模型
内部含有大量公式，所以先列一下大纲:
1. IMU 连续模型
2. 误差状态模型-Rotation
   
----
1. IMU 连续模型
   完美模型，也就是没有任何误差的模型
   $$
    \left[
        \begin{matrix}
            R^{l+1}_G\\
            v^{l+1}_G\\
            p^{l+1}_G
        \end{matrix}
        \right]
    = 
    \left[
    \begin{matrix}
        R^{l}_GR^{l+1}_{l}\\
        v^{l}_G + R^l_G\int^{t_{l+1}}_{t_l}R_l^{\tau}a_{\tau}d{\tau} + g(t_{l+1}-t_{l})\\
        p^{l}_G + v^{l}_G(t_{l+1}-t_{l}) + R^l_G\int^{t_{l+1}}_{t_l}\int^s_{t_l}R_l^{\tau}a_{\tau}d{\tau}ds(=\int^{t_{l+1}}_{t_l}v_G(s)ds) + \cfrac{1}{2}g(t_{l+1}-t_{l})^2
    \end{matrix}
    \right]
   $$
2. 误差状态模型-Rotation
   $$
    \left\{
    \begin{aligned}
        R^{l+1}_G &= &R^{l}_GR^{l+1}_{l}\\
        R&=& \hat{R}(1- [\delta \theta]_×)
    \end{aligned}
    \right.
   $$
   打括号里面的第二行的意思，就是真值= 测量-存在的error

   这里需要把$SO3$ 转变为$so3$
   $$
    nominal:\\
     R^{l+1}_G = R^{l}_GR^{l+1}_{l}\\
    true:\\
    \begin{aligned}
    R^{l+1}_G &= R^{l}_GR^{l+1}_{l}  \\
    \hat{R^{l+1}_G}(1- [\delta \theta^{l+1}_G]_×)&= \hat{R^{l}_G}(1- [\delta \theta^{l}_G]_×)\hat{R^{l+1}_{l}}(1- [\delta \theta^{l+1}_{l}]_×)\\
    
    \end{aligned}
   $$

