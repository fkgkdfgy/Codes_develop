<!--
 * @Author: Liu Weilong
 * @Date: 2021-02-06 22:52:42
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-02-20 17:24:24
 * @Description: 
-->
### 1. 图像去畸变
最终结果:
![](./../pictures/undistortion.png)

### 2. 双目视差
最终结果:
![](./../pictures/disparity.png)

### 3. 矩阵微分
1. 
$$
    \frac{dAx}{dx} = A
$$
2. 
$$
    \frac{dx^TAx}{dx} = 2A
$$
3. 
<font color = "Red">
作业修改的部分
</font><br>
使用矩阵迹的一个性质
$$
    \operatorname{tr}(ABC) = \operatorname{tr}(BCA)
$$

最终可以得到
$$
        x^TAx=\operatorname{tr}(x^TAx) = \operatorname{tr}(Axx^T)
$$

### 4. GN 优化
最终结果:
![](./../pictures/result.png)

### 5. 批量估计
1. 给出H的具体形式
$$
    x_3 = x_2 +v_3\\
    x_2 = x_1 +v_2\\
    x_1 = x_0 +v_1\\
    y_3 = x_3 \\
    y_2 = x_2 \\
    y_1 = x_1 \\
    H= \left[
        \begin{matrix}
            -1&1&0&0\\
            0&-1&1&0\\
            0&0&-1&1\\
            0&1&0&0\\
            0&0&1&0\\
            0&0&0&1\\
        \end{matrix}
    \right]
$$
2. W的具体数值
$$
    W= \left[
        \begin{matrix}
            Q^{-1}&0&0&0&0&0\\
            0&Q^{-1}&0&0&0&0\\
            0&0&Q^{-1}&0&0&0\\
            0&0&0&R^{-1}&0&0\\
            0&0&0&0&R^{-1}&0\\
            0&0&0&0&0&R^{-1}\\
        \end{matrix}
    \right]
$$
3. 存在唯一解，因为
   $$
    H^TWH
   $$
   满秩



