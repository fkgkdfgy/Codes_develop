<!--
 * @Author: Liu Weilong
 * @Date: 2021-05-06 14:06:43
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-05-06 16:10:59
 * @FilePath: /Codes/other.md
 * @Description: 
-->
### 地面约束 EdgeGroundConstraintZRP

Residual:
$$
    r_1 = e_3^Tt_{bc} - e_3^{T}(-R_{cw}^{T}t_{cw}) 
    \\
    r_2 = \left[
        \begin{aligned}
            -e_1^T\\-e_2^T
        \end{aligned}\right]In([R_{bc}R_{cw}]^{T})
$$
Jacobian:

Residual 1
$$
    In(R_{cw}) = In(exp(\theta_{cw})) = \theta_{cw}\\
    \begin{aligned}
    J^{r_1}_T &= 
    \left[
        \begin{matrix}
           \cfrac{\partial r_1}{\partial \theta_{cw}} &
           \cfrac{\partial r_1}{\partial t_{cw}}
           
        \end{matrix}
    \right]
    \\
    &=
    \left[
        \begin{matrix}
            \cfrac{\partial {(-e_3^TR_{cw}^Texp(-\delta\theta)t_{cw})}}{\delta{\theta}}&
           e_3^TR_{cw}^T
        \end{matrix}
    \right]
   \\
    &=
    \left[
        \begin{matrix}
         e_3^T[R_{cw}^Tt_{cw}]^×&
           e_3^TR_{cw}^T
        \end{matrix}
    \right]        
    \end{aligned}

$$

Residual 2
$$

    \begin{aligned}
    J^{r_2}_T &= 
    \left[
        \begin{matrix}
           \cfrac{\partial r_2}{\partial \theta_{cw}} &
           \cfrac{\partial r_2}{\partial t_{cw}}
        \end{matrix}
    \right]
    \\
    &=
        \left[
        \begin{matrix}
           \left[
        \begin{aligned}
            -e_1^T\\-e_2^T
        \end{aligned}\right]\cfrac{\partial In([R_{bc}exp(\delta \theta)R_{cw}]^{T})}{\partial \delta \theta}&
          0^{1×3}
        \end{matrix}
    \right]
    \\
    & = 
     \left[
        \begin{matrix}
           \left[
        \begin{aligned}
            -e_1^T\\-e_2^T
        \end{aligned}\right]\cfrac{\partial In(R_{cw}^TR_{bc}^Texp(-R_{bc}\delta \theta))}{\partial \delta \theta}&
          0^{1×3}
        \end{matrix}
    \right]
    \\
    &=
     \left[
        \begin{matrix}
           \left[
        \begin{aligned}
            -e_1^T\\-e_2^T
        \end{aligned}\right]\cfrac{\partial In(R_{bw}^Texp(-R_{bc}\delta \theta))}{\partial \delta \theta}&
          0^{1×3}
        \end{matrix}
    \right]
    \\
    & = 
         \left[
        \begin{matrix}
          \left[
        \begin{aligned}
            e_1^T\\e_2^T
        \end{aligned}\right]J^{-1}_r(\theta_{bw})R_{bc} &
          0^{1×3}
        \end{matrix}
    \right]
    \end{aligned}
$$


### 线特征
Residual:
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
            \pi(KTp_s) \\
            \pi(KTp_e)
        \end{matrix}
    \right]
   $$
Jacobian:
对两点进行jacobian
$$
    \begin{aligned}
    J^r_p &= \left[
        \begin{matrix}
          \cfrac{\partial r}{\partial p_s} &0 \\
          0 &\cfrac{\partial r}{\partial p_e}
        \end{matrix}
        \right]
    \\
    &=
    \left[
        \begin{matrix}
          n^TJ^{uv}_{xyzs}R_{cw} &0 \\
          0 & n^TJ^{uv}_{xyze}R_{cw}
        \end{matrix}
        \right]
    \end{aligned}
    \\
    \begin{aligned}
    J^{uv}_{xyzs} &= 
    \left[
        \begin{matrix}
          \cfrac{\partial(f_x\cfrac{x}{z}+c_x)}{\partial p} \\
          \cfrac{\partial(f_x\cfrac{x}{z}+c_y)}{\partial p}
        \end{matrix}
        \right]|_{p = p_s}
    \\
    & = 
      \left[
        \begin{matrix}
          \cfrac{f_x}{z} & 0 & -f_x\cfrac{x}{z^2} \\
          0&\cfrac{f_y}{z}  & -f_y\cfrac{y}{z^2}
        \end{matrix}
        \right]|_{p = p_s}      
    \end{aligned}
$$
对位姿继续Jacobian
$$
 \begin{aligned}
    J^r_T &= \left[
        \begin{matrix}
          \cfrac{\partial r}{\partial T}  
        \end{matrix}
        \right]
    \\
    &=
    \left[
        \begin{matrix}
          n^T\cfrac{\partial(\pi(K(exp(\delta \theta)R_{cw}p_s+t_{cw}+\delta t)))}{\partial [\delta \theta ,\theta t]}\\
          n^T\cfrac{\partial(\pi(K(exp(\delta \theta)R_{cw}p_e+t_{cw}+\delta t)))}{\partial [\delta \theta ,\theta t]}
        \end{matrix}
        \right]
    \\
    & = 
        \left[
        \begin{matrix}
         -n^T J^{uv}_{xyzs}[R_{cw}p_s]^× & n^T J^{uv}_{xyzs} I\\
         n^T J^{uv}_{xyze}[R_{cw}p_e]^×& n^T J^{uv}_{xyze
         } I
        \end{matrix}
        \right]
    \end{aligned}


$$