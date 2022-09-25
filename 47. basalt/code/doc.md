<!--
 * @Author: Liu Weilong
 * @Date: 2021-05-21 09:44:12
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-21 11:08:14
 * @Description: 
-->
### 奇怪的pose graph 优化
下面是奇怪的写法,统一左乘更新

emmm T 写成R了 勉强看一下吧



$$
\begin{aligned}
    residual &= [log_G(R_m R_1^T R_2)]^\vee
    \\
    J^r_{R_2} &= \cfrac{[log_G[R_mR_1^Texp_G(\delta \theta_2)R_2 (R_m R_1^T R_2)^T]]^\vee}{\delta \theta_2}
    \\
    & = \cfrac{[log_G[R_mR_1^Texp_G(\delta \theta_2)R_2 R_2^T R1 R_m^T]]^\vee}{\delta \theta_2}
    \\
    & = \cfrac{[log_G[R_mR_1^Texp_G(\delta \theta_2)(R_mR_1^T)^T]]^\vee}{\delta \theta_2}
    \\
    & = Adj(R_mR_1^T)
    \\
    J^r_{R_1} &= \cfrac{[log_G[R_mR_1^Texp_G(-\delta \theta_1)R_2 (R_m R_1^T R_2)^T]]^\vee}{\delta \theta_1}
    \\
    & = -Adj(R_mR_1^T)
    
\end{aligned}
    
$$



