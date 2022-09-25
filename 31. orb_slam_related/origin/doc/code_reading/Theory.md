
### 关于ORB2 当中经常使用的bf
bf 这里的意思是 $baseline×f_{x}$ <br>
在深度估计中,<br>
这里的假设应该是，左右相机水平<br>
$$
    \cfrac{z(m)}{baseline(m)} = \cfrac{f_x(pixel)}{disparity(pixel)} = \cfrac{f_x(pixel)}{p_{xl}-p_{xr}(pixel)}
$$
经过结合之后，
$$
    z(m) = \cfrac{bf(pixel*m)}{disparity(pixel)}
$$
在理清各个变量的单位之后，整个逻辑就变得清晰了起来<br>
由此我们引出ORB2 Pose Optimization 的误差项
### ORB2 Stereo PoseOptimzation 误差项
$$
    \begin{aligned}
    d_{pix} &= f(Rp+t)
    \\
    d_{dis} &= p_{xr} - p_{xl} + \cfrac{bf}{z}
    \\
    & =  p_{xr} - p_{xl} + disparity
    \end{aligned}
$$
$d_{pix}$ 来自于左目， $d_{dis}$ 来自于左右目和深度的约束