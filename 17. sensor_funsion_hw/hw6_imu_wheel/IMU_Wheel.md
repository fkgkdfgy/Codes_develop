## IMU Wheel 融合 

这是是采用比较简单的方式<br>
IMU的陀螺仪输出的角速度结合轮式计的线速度进行融合<br>
这里假设IMU和轮式计之间是通过 $T^{imu}_{wheel}$ 进行固练<br>
$v_{wheel}$ 代表轮式计输出的速度<br>
$v_{imu} =T^{imu}_{wheel}v_{wheel}$
$v_{imu}$ 代表转化到imu系的速度<br>
<br>


### 状态量递推

<font color = "Red"> 2021.4.12 添加
基础内容:
$$
    \begin{aligned}
     R\theta \times R^T = (R\theta)×  
     \\
     Rexp(\theta)R^T = exp(R\theta)    
    \end{aligned}
$$
这个，之后的求导过程中，会被不断地被使用。

写在这里，是因为自己可能会忘记这些基础的公式。
</font>

之后，就可以套入预积分的公式
$$
    \alpha_{b_ib_{k+1}} = \alpha_{b_ib_k} + exp(\theta_{b_ib_{k}})v_{imu}\Delta{t} \\
    \theta_{b_ib_{k+1}} = In(exp(\theta_{b_ib_{k}})exp[(\omega_m - b_{gk})\Delta{t}])\\
    b_{gk+1} = b_{gk}
$$
为了方便之后，对于Bias 展开的需求<br>
这个通过对每一个IMU进行泰勒展开，合并起来，最终得到用于对Bias泰勒展开的$J_{ij}$<br>
因为$\alpha$ 和 $b_g$ 已经是一个线性的关系，所以只对 $\theta$ 展开即可


主体推导:

$$
        \begin{aligned}
        \theta_{b_ib_{k+1}} &= In(exp(\theta_{b_ib_{k}})exp[(\omega_m - b_{gk})\Delta{t}])
        \\
        &= J^{\theta_{b_ib_{k+1}}}_{\theta_{b_ib_{k}}}(\theta_{b_ib_{k}}-\hat{\theta_{b_ib_{k}}})+J^{\theta_{b_ib_{k+1}}}_{b_{gk}}(b_{gk}-\hat{{b_{gk}}})+c
        \end{aligned}
$$
$$
    \begin{aligned}
    \theta_{whole}&=In(exp(\hat{\theta_{b_ib_{k}}})exp[(\omega_m - \hat{b_{gk}})\Delta{t}])\\
    J^{\theta_{b_ib_{k+1}}}_{\theta_{b_ib_{k}}}&=\cfrac{d\theta_{b_ib_{k+1}}(\delta{\theta})}{d\delta{\theta}} \\&= \frac{In(exp(\hat{\theta_{b_ib_{k}}})exp(\delta{\theta})exp[(\omega_m - \hat{b_{gk}})\Delta{t}])}{d\delta\theta}\\
    &=J^{-1}_r(exp(\theta_{whole}))(exp[(\omega_m - \hat{b_{gk}})\Delta{t}])^T
    \end{aligned}
$$
$$
    \begin{aligned}
    J^{\theta_{b_ib_{k+1}}}_{b_{gk}} &= \cfrac{d\theta_{b_ib_{k+1}}(\delta{b_{gk}})}{d\delta{b_{gk}}}\\&=\frac{In(exp(\hat{\theta_{b_ib_{k}}})exp[(\omega_m - \hat{b_{gk}}-\delta{b_{gk}})\Delta{t}])}{d\delta{b_{gk}}}\\
    &=-J^{-1}_r(exp(\theta_{whole}))J_r(exp[(\omega_m - \hat{b_{gk}})\Delta{t}])\Delta{t}
    \end{aligned}
$$

$c$ 是常数项

之后就有了
$$
    X_{k+1} = F_{k}X_{k} + ... 
$$
在不断的迭代之后，$J^{\alpha}_{b_g}$ 等 就可以从迭代得到的$J$ 里面取了。


### 方差估计

因为最后的估计是最大后验
$$
    p(x|z) = p(z|x)p(x)
$$

$p(x)$ 是先验部分，属于另外一个误差项，这里就不用关心了。
$p(z|x)$ 当高斯分布来看
$$
    In(p(z|x)) = (z-Cx)^{T}\Sigma^{-1}(z-Cx)
$$
从之前的课程来看<br>
$$z-Cx = \delta{z}$$
且这里$C$是一个单位阵
$$\delta{z} = \delta{x_{imu}}$$
所以,可以使用之前在ESKF 的时候的误差状态分析,在这里对方差进行分析。
$$
    \begin{aligned}
    \dot{p} &= exp(\theta)v_{imu}\\
    \dot{p} + \dot{\delta{p}} &=exp(\theta)exp(\delta{\theta})(v_{imu}+n_{v})\\
    \dot{\delta{p}} &= -exp(\theta)[v_{imu}]×\delta{\theta} + exp(\theta)n_v\\
    \dot{exp(\theta)} &= exp(\theta)[\omega_m - b_g]× \\
    \dot{(exp(\theta)exp(\delta{\theta}))}&=exp(\theta)exp(\delta{\theta})[\omega_m - b_g-\delta{b_g}+n_{\omega}]×\\
    \dot{\delta{\theta}} &= -[\omega_m-b_g]×\delta{\theta}-\delta{b_g}+n_w \\
    \dot{bg} & = 0\\
    \dot{(bg+\delta{b_g})} &= n_{gwalk}\\
       \dot{\delta{b_g}} &= n_{gwalk}
    \end{aligned}
$$

得到

$$
    \dot{\delta{X}} = F\delta{X}+ BN
    \\
    \begin{aligned}
    \delta{X}_{k+1} &= \delta{X}_{k} +  \dot{\delta{X}_{k}}\Delta{t}
    \\&=(I+F\Delta{t})\delta{X}_k + BN\Delta{t}
    
    \end{aligned}
$$

#### 2021.2.3 添加
$$
\begin{matrix}
    &\dot{\delta{X}} = F\delta{X}+BN
    \\
    & \delta{X}= (e^{Ft}+D)
    \\
    &\dot{\delta{X}} = F((e^{Ft}+D))+BN
    \\
    &-FD=BN
\end{matrix}


$$



不断进行迭代进行方差计算了。

### r求导
最后就是对于$r$的求导
$$
    \begin{aligned}
    r_{\alpha_{bjbi}} &= p_{bj}-p_{bi}-\alpha_{bjbi}-J^{\alpha}_{b_{gbi}}(b_{gbi}-\hat{b_{gbi}})\\
    \cfrac{\partial{r_{\alpha_{bjbi}}}}{\partial{p_{bj}}}&=I
    \\
    \cfrac{\partial{r_{\alpha_{bjbi}}}}{\partial{p_{bj}}}&=-I
    \\
    \cfrac{\partial{r_{\alpha_{bjbi}}}}{\partial{b_{gbi}}}&=-J^{\alpha}_{b_{gbi}}
    \\
    r_{\theta_{bjbi}}&=In[R_j^TR_iexp(\theta_{b_jb_i}+J^{\theta_{b_jb_i}}_{b_{gi}}(b_{gbi}-{b^{linearized}_{gbi}}))]
    \\
    \theta_{whole} &= In[\hat{R_j}^T\hat{R_i}exp(\theta_{b_jb_i}+J^{\theta_{b_jb_i}}_{b_{gi}}(\hat{b_{gbi}}-{b^{linearized}_{gbi}}))
    \\
    \cfrac{\partial{\theta_{bjbi}}}{\partial{R_j}}&= -J^{-1}_{l}(exp(\theta_{whole}))
    \\
    \cfrac{\partial{\theta_{bjbi}}}{\partial{R_i}}&= J^{-1}_{r}(exp(\theta_{whole}))(exp(\theta_{b_jb_i}+J^{\theta_{b_jb_i}}_{b_{gi}}(\hat{b_{gbi}}-{b^{linearized}_{gbi}})))^T
    \\
    \cfrac{\partial{\theta_{bjbi}}}{\partial{b_{gki}}}&= J^{-1}_{r}(exp(\theta_{whole}))J_{r}(exp(\theta_{b_jb_i}+J^{\theta_{b_jb_i}}_{b_{gi}}(\hat{b_{gbi}}-{b^{linearized}_{gbi}})))J^{\theta_{b_jb_i}}_{b_{gi}}
    \end{aligned}
$$