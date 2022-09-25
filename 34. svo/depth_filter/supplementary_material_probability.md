<!--
 * @Author: Liu Weilong
 * @Date: 2021-02-25 10:59:33
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-03 11:21:45
 * @FilePath: /3rd-test-learning/34. svo/depth_filter/supplementary_material_probability.md
 * @Description: 
-->
### 各种在Depth Filter 中，能用到的分布
1. 从Bernouli 到binomial<br>
   Bernouli 分布<br> 单次事件，描述01的概率情况
   $$
    p(k|\theta) = p^k(1-p)^{1-k}
   $$
   k只有0和1，p是当k=1的时候的概率<br>
   Binomial 分布<br> 描述多次独立的Bernouli分布，出现次数的概率<br>
   从 naive definition probability (数数)来看,可以抽象成5个人，从装有4个黑球和6个白球的盒子里面，进行有放回的取球。<br>
   问 出现 2次黑球的概率是多少？
   $$
   \begin{aligned}
    p(c_2|\theta ,n) &= \cfrac{C^5_22^43^6}{5^10}
    \\
    & = C^5_20.4^20.6^3
   \end{aligned}
   $$
   进一步进行抽象
   $$
    p(c_a|\theta) = C^n_ap^a(1-p)^{n-a}
   $$
2. 从 Binomial 到 Beta 分布<br>
   Beta分布<br>
   从均值开始，Beta的均值直接来自于二值测试<br>
   假如历史过过程中，0发生过$\alpha$次1发生过$\beta$次,Beta分布的均值就是$\cfrac{a}{a+b}$
   $$
    \operatorname{Beta(a,b)} = p(\theta|a,b) = \cfrac{\theta^{a-1}(1-\theta)^{b-1}}{B(a,b)}\propto \theta^{a-1}(1-\theta)^{b-1}
   $$
   $B(a,b)$只是为了进行归一化
   Beta分布是将在1. 中固定的$\theta$ 用一个分布来进行表示。a和b 就是01的假定次数。<br>
   <font color="Red">2021.2.16添加:</font><br>
   $$
   B(a.b) = \frac{\Gamma(a)\Gamma(b)}{\Gamma(a+b)}
   \\
   \Gamma(a) = a!
   $$
3. Beta分布的Conjugate Piror to Binomial属性<br>
   $$
   \begin{aligned}
       p(\theta|data) &= \cfrac{p(data|\theta)p(\theta)}{p(data)}
        \\
        & \propto p(data|\theta)p(\theta)
        \\
        & \downarrow  p(data|\theta) = C^n_ap^a(1-p)^{n-a} \rightarrow Binomial&Distribution
        \\
        & \propto \theta^a(1-\theta)^{n-a} \theta^{\alpha-1}(1-\theta)^{\beta -1}
        \\
        &= \theta^{a+\alpha -1}(1-\theta)^{n-a+\beta-1}
   \end{aligned}
   $$
   依旧是一个Beta 分布
4. Gamma 分布<br>
   Gamma分布对于正整数来看 = $n!$<br>
   也就是n个人有多少种排列方式<br>
5. B分布<br>
   B分布:
   $$
      B(a,b) = \cfrac{\Gamma(a)\Gamma(b)}{\Gamma(a+b)}
   $$
