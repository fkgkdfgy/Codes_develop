<!--
 * @Author: Liu Weilong
 * @Date: 2021-03-01 16:34:18
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-04-16 17:23:13
 * @FilePath: /Codes/34. svo/edgelet/doc.md
 * @Description: 
-->

### edgelet :
1. 定义:具有连续且强的灰度梯度的小线段<br>
2. 长线的缺点:容易被遮挡\慢<br>
3. 空间表示: 一个?
4. 提取： Gaussian 滤波， Scharr XY发想梯度， Canny 提边缘
5. (来自SVO_EDGLET)优化的部分，直接法当作edgelet进行，而是在Alig1D的时候，对Jacobian ，公式如下:
$$
   \sum_{\Delta u\in P} [I_1(\boldsymbol{u}+\delta t \boldsymbol{n}+\Delta u ) - I_2(\boldsymbol{u}+\Delta u) +m]^2 = \sum_{\Delta u\in P} r^2 
    \\
    \begin{aligned}
    J^r_{\delta t} &= \cfrac{dI}{d(u+\delta t \boldsymbol{n}+\Delta u)}\cfrac{d(u+\delta t \boldsymbol{n}+\Delta u)}{d(\delta t)}
    \\
    & = \left[
        \begin{matrix}
            I_x & I_y
        \end{matrix}
        \right]
        \left[
        \begin{matrix}
            n_x\\
            n_y
        \end{matrix}
        \right]  
    \\
    J^r_{m} &= 1;  
    \end{aligned}
   \\
$$




