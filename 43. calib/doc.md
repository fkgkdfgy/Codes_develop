<!--
 * @Author: Liu Weilong
 * @Date: 2021-04-08 20:11:49
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-04-17 19:01:47
 * @Description: 
-->
### 梗概(外参标定会直接和客观性产生关系)
1. rot 外参标定
2. imu gyro bias 标定
3. 机器人学中的状态估计，旋转误差状态分析
4. SO3 imu 预积分模块
5. 图优化因子求导



======
1. rot 外参标定
   $$
   \begin{aligned}
           R^{a2}_{a1} &= R^{b}_a R^{b2}_{b1} R^{a}_{b}
    \\
     &= R^{b}_a R^{b2}_{b1} (R^{b}_a)^T
     \\
     & = exp(R^b_a \theta_b)
     \\
     & = exp(\theta_a)
   \end{aligned}
   $$
   标定目标就是$R^{b}_a$

   $$
    q_{a1}^{b2}=q^{a2}_{a1}q^{b}_a= q^b_aq^{b2}_{b1}
    \\
   L(q^{a2}_{a1})q^{b}_a - R(q^{b2}_{b1})q^b_a = 0
   \\
   [L(q^{a2}_{a1}) - R(q^{b2}_{b1})]q^b_a = 0
   $$
   SVD 求解

   

2. imu gyro bias 标定
   
   基础推导
   $$
    RR^T=I \rightarrow \dot{R}R^T + R\dot{R}^T = 0 \rightarrow \dot{R}  = -\omega × R\\
    R^TR=I \rightarrow \dot{R}^TR +R^T\dot{R} = 0\rightarrow \dot{R}^T=(R\omega×)^T   \\
    

   $$
   误差状态分析<font color = "Red">见3.</font>
   
   主体内容:

   imu的gyro预积分部分
   2.a 公式递推
   $$
   X_k+1 = f_k(X_k,b_k); 
   \\
   \begin{aligned}
      J^{X_{k+2}}_{b_k} &=J^{f_{k+1}}_{X_{k+1}}( J^{f_k}_{b_k}(b_k-\hat{b_k}))+ J^{f_{k+1}}_{b_{k+1}}(b_{k}-\hat{b_{k}})\\
      &=J^{X_{k+2}}_{X_{k+1}}( J^{X_{k+1}}_{b_k}(b_k-\hat{b_k}))+ J^{X_{k+2}}_{b_{k+1}}(b_{k}-\hat{b_{k}})
      \\
      &=( J^{X_{k+2}}_{b_k}(b_k-\hat{b_k}))+ J^{X_{k+2}}_{b_{k+1}}(b_{k}-\hat{b_{k}})      
   \end{aligned}
   $$
   具体推导<font color = "Red">见 32. vins_related/origin/VINS_MONO/Theory.md </font>
   $$
      In(\prod exp(\omega_i + b + n_i))
      = In(\prod exp(\omega_i + \hat{b} + n_i)) + J_{b}(b-\hat{b})
   $$

   然后，组建最小二乘单次求解即可得到b。(VINS的一种近似求解) 这里使用的是 Cholesky 的求解方式(Ax = b A是正定(方阵，对称+ determiant>0))
   
   实际上，应该是进行反复迭代更新 exp 的。

3. 机器人学中的状态估计，旋转误差状态分析
   P227
   $$
   nominal:\\
   \dot{C} = \omega \times C\\
   true:\\
   \cfrac{d}{dt}(1+\delta \theta\times)C = (\omega + \delta \omega)\times (1+\delta \theta \times)C \\
   left:\\
   
   \dot{\delta \theta×}C+(1+\delta \theta \times)\omega \times C \\
   left+right:\\
   \dot{\delta \theta×}+(1+\delta \theta \times)\omega \times = (\omega + \delta \omega)\times (1+\delta \theta \times)
   \\
   \dot{\delta \theta×} = (\omega + \delta \omega)\times (1+\delta \theta \times) - (1+\delta \theta \times)\omega \times
   \\
   formula 1：\\
   V1 \times V2 \times - V2 \times V1 \times = (V1 \times V2)\times
   \\
   continue:\\
   \begin{aligned}
      \dot{\delta \theta×} &= (\omega + \delta \omega)\times (1+\delta \theta \times) - (1+\delta \theta \times)\omega \times
      \\
      &= \omega \times + \omega \times \delta{\theta}\times + \delta{\omega}\times + \omega \times -\delta{\theta}\times \omega \times
      \\
      &=(\omega×\delta{\theta})\times + \delta{\omega}\times      
   \end{aligned}
   $$
   这里存在的符号正负号转变。认为只是符号定义的问题

4. 预积分模块

5. 图优化因子求导

$$
   In(R_j) = In(R_i exp(\phi_m))= In(exp(\phi_i)exp(\phi_m)) = In(exp(\phi_{whole}))
   \\
   \cfrac{d R_j}{dR_i} =\operatorname{lim} \cfrac{In(exp(\phi_{whole} + J^{-1}_r(\phi_{whole})exp(\phi_m)^T\delta\phi))-In(exp(\phi_{whole}))}{\delta\phi}
   \\
   =J^{-1}_r(\phi_{whole})exp(\phi_m)^T
$$