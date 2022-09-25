

### ESKF推导

ESKF 过程推导

IMU 角度 $\theta$ 推导


$$
    \dot{\delta{\theta}} = -(\omega_m-b_g)×\delta{\theta} +n_g -\delta{b_g}
$$

速度 误差状态更新 
$$
    \dot{\delta{v}} = C^{n}_b(-\delta{b_a}+n_a)+
    C^{n}_b\delta{\theta}×(a_m-b_a)
$$

位置 误差状态更新
$$
    \dot{\delta{p}} = \delta{v}
$$

角度 观测

$$
    R_{measure} = I * R_{true}\\
    R_m(I+\delta{\theta_{m}}×) = R_{true}[I+(\delta{\theta_{true}}+n_z)×] \\
    \delta{\theta_m} = \delta{\theta_{true}}+n_z

$$
从这里可以看出<br>
$R^{true}_{measure} = R_m(I+\delta{\theta_{m}}×)$ 这个是来自传感器的数据
$R^{nominal}_{measure} = I * R_{true}$ 这个是来自IMU 递推的状态量推断出的观测 

$$
    \delta{\theta_{m}} = In((R^{nominal}_{measure})^{T}R^{true}_{measure})
$$

速度观测

$$
    v_{measure} = v_{true}\\
    v_{measure} +\delta{v_{measure}} = v_{true} + \delta{v_{true}}\\
    \delta{v_{measure}} = \delta{v_{true}}
$$



### SO3 上的 EKF