<!--
 * @Author: Liu Weilong
 * @Date: 2021-02-04 07:35:18
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-07-06 11:14:52
 * @Description: 
-->
### 线性系统速记 小手册
1. 定常系统的通解
   $$
   \begin{aligned}
    &\dot{x} = Ax + Bu\\
    & x = e^{At}x(0)+\int^{t}_0 e^{A(t-\tau)}Bu(\tau)d\tau
    \end{aligned}
   $$
   离散化：
   $$
    \\
    e^{At} = I+At+\frac{1}{2}(At)^2+\frac{1}{3!}(At)^3 ...+\frac{1}{n!}(At)^{n}...
   \\
   \int^{t}_0 e^{A(t-\tau)}Bu(\tau)d\tau = e^{A(t-t)}Bu(t) \Delta t=Bu(t)\Delta t

   $$
   y的部分
   $$


   \begin{aligned}
    &y = Cx + Du\\
    &y = C[e^{At}x(0)+\int^{t}_0 e^{A(t-\tau)}Bu(\tau)d\tau]+Du
    \end{aligned}
     
   $$
   $e^{At}$ 可以通过CH 公式写成解析解
   
2. 定常系统求解方法<br>
   系统公式:<br>
   $$
   \dot{x}(t) = Ax(t)+Bu(t)
   \\
   y(t) = Cx(t)+Du(t)
   $$
   x 部分的推导:<br>
   $$
   e^{-At}\dot{x}(t) - e^{-At}Ax(t) = e^{-At}Bu(t)
   \\
   \frac{d(e^{-At}x(t))}{dt} = e^{-At}Bu(t)
   \\
   e^{-A\tau}x(\tau)|^t_{\tau=0} = \int^t_0e^{-A\tau}Bu(\tau)d\tau
   \\
   e^{-At}x(t) - x(0) =\int^t_0e^{-A\tau}Bu(\tau)d\tau
   \\
   x(t) = e^{At}x(0) + \int^t_0e^{A(t-\tau)}Bu(\tau)d\tau
   $$
   y 部分的推导:<br>
   $$
      \begin{aligned}
      y(t) &= Cx(t)+Du(t)
      \\
      & = C[e^{At}x(0) + \int^t_0e^{A(t-\tau)}Bu(\tau)d\tau] + Du(t)
      \end{aligned}
   $$
