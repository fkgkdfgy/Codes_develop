<!--
 * @Author: Liu Weilong
 * @Date: 2021-06-13 13:56:38
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-14 08:57:20
 * @Description: 
-->
### 遗留问题
1. basalt的nfr问题
2. basalt对于线性化点的维护
3. basalt nullspacecheck

-----
2. 对于线性化点的维护
   
   BASALT当中支持的是，如果一个点已经被线性化,那么这个点的线性化点就不会再改变，这种方法的确很大程度上保证了计算量的问题。 (但是如果有回环呢？)
   
   2.1 marginalization offset

   这里其实主要是解释 边缘化 8. 有一个offset 的问题

   要说明这个问题，要从线性化开始讲起


   因为Basalt 的线性化操作，所以，一旦一个变量进行线性化之后，jacobian 的线性化点基本就固定死了。

   之后有关这个参数的线性化都会在这个线性化点上进行展开了。

   写成公式之后，出现如下的情况

   $$
        \sum[r-f(x)]^T\Sigma^{-1}[r-f(x)]
        \\
        = \sum[r-f(x_0) - F(x-x_0)]^T\Sigma^{-1}[r-f(x_0) - F(x-x_0)]
   $$

   因为是高斯分布，最后通过MAP 一定可以得到一个这样的X
   $$
     X \sim N(x_0+\delta,\Sigma)
   $$
   写成信息矩阵的形式
   $$
     X \sim N^{-1}(\Omega(x_0+\delta),\Omega)
     \\
     X \sim N^{-1}(\xi,\Omega)
   $$
   
   最终的 abs_H 和 abs_b 代表的意思也就是 $\Omega$ 和 $\Omega(x_0+\delta)$
   
   这也是在进行边缘化之前一定会得到这样的一个东西。

   然后进行边缘化之后，

   会得到新的$\Omega$ 和 $\xi$

   通过边缘化公式我们可以知道
     $$
    p(a|b) \sim N(a-C^TB^{-1}b,A-C^TB^{-1}C)
    \\
    p(a|b) \sim N^{-1}(b_{marg},H_{marg})
    $$
    因为b 是被边缘化的数值 a 保留下来。

    所以可以将公式进一步展开
    $$
     p(a|b) \sim N(x_{0keep}+\delta_{keep}- C^TB^{-1}(x_{0marg}+\delta_{marg}),A-C^TB^{-1}C)
     \\
     p(a|b) \sim N^{-1}(H_{marg}[x_{0keep}+\delta_{keep}- C^TB^{-1}(x_{0marg}+\delta_{marg})],H_{marg})
    $$
    这里为了可以继续使用 $x_0$ 进行线性化。
    所以需要把$\delta$ 剔除
    
    就有了代码中的公式

    marg_b -= marg_H * delta;(delta =  $\delta_{keep}$)

    关于先验变化的问题，基本已经明白了，是优化形式导致的

