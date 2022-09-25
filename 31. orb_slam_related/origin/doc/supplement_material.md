<!--
 * @Author: Liu Weilong
 * @Date: 2021-01-29 09:58:50
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-02-23 15:11:22
 * @FilePath: /3rd-test-learning/31. orb_slam_related/origin/doc/supplement_material.md
 * @Description: 
 * 
-->
### EPnP(四个控制点)

reference:
https://blog.csdn.net/jessecw79/article/details/82945918#control_points_64 \
https://zhuanlan.zhihu.com/p/59070440

------
<font color = "Red"> 目的：从3D-2D 匹配关系里面得到Transform</font><br>
基础1：
$$
    p_i^w = \sum_{j=1}^{4}\alpha_{ij}c_j^{w}\\
    \sum_{j=1}^{4}\alpha_{ij} = 1
$$
存在四个控制点 可以通过$\alpha_{ji}$ 组合成任意的$p^w$<br>
基础2：
$$
    p_i^c = \left[
        \begin{matrix}
            R & t
        \end{matrix}
    \right]
    \left[
        \begin{matrix}
           p_i^w \\1
        \end{matrix}
    \right]
$$
从基础1我们可以得到一个关系
$$
    \begin{aligned}
    p_i^c &= \left[
        \begin{matrix}
            R & t
        \end{matrix}
    \right]
    \left[
        \begin{matrix}
           p_i^w \\1
        \end{matrix}
    \right]
    =\left[
        \begin{matrix}
            R  & t
        \end{matrix}
    \right]
        \left[
        \begin{matrix}
           \sum_{j=1}^{4}\alpha_{ij}c_j^{w} \\
           \sum_{j=1}^{4}\alpha_{ij}
        \end{matrix}
    \right]
    \\
    &=\sum_{j=1}^{4}\alpha_{ij}(Rc_j^{w}+t)
    \\
    &=\sum_{j=1}^{4}\alpha_{ij}c_j^{c}
    \end{aligned}
$$
<font color="Red">可见$\alpha$关系从world 坐标系带到了camera坐标系,这里是从2D算出$c^{c}$的关键</font>


-----
正文开始：<br>
1. 找到控制点$c_j^{w}$
2. 找到$\alpha_{ij}$
3. 算出控制点$c_j^{c}$
4. 求解Transform(ICP)

------
1. 找控制点$c_j^{w}$<br>
   意义上只要满足以下公式就可以
   $$
   rank(
   \left[
    \begin{matrix}
        c_1^{w} &c_2^{w}&c_3^{w}&c_4^{w}\\
        1&1&1&1
    \end{matrix}
    \right])=4
   $$
   一般求解方式
   $$
    c_1^{w}= \cfrac{1}{n}\sum_ip_i^w\\
    c_{2-4}^w = \operatorname{EigenVector}(   \left[
    \begin{matrix}
        p_1-c_1\\
        \vdots\\
        p_n-c_1
    \end{matrix}
    \right]^T 
    \left[
    \begin{matrix}
        p_1-c_1\\
        \vdots\\
        p_n-c_1
    \end{matrix}
    \right])+c_1
   $$
2. 找到$\alpha_{ij}$
   $$
   C^{w}=\left[
    \begin{matrix}
        c_1^{w} &c_2^{w}&c_3^{w}&c_4^{w}\\
        1&1&1&1
    \end{matrix}
    \right]
    \\
    C^w\left[
    \begin{matrix}
        \alpha_{i1}\\
        \alpha_{i2}\\
        \alpha_{i3}\\
        \alpha_{i4}\\
    \end{matrix}
    \right]=p_i^w
   $$
   求逆之后进行求解即可
3. 算出控制点$c_j^{c}$ <br>
   参考reference
4. 求解Transform(ICP)<br>
   到这里我们<br>
   从3D点中得到 $\alpha_{ij}$  和$c_j^{w}$<br>
   从2D点中得到$c_j^{c}$
   根据基础二，可以得到$p_i^c$和$p_i^w$<br>
   且存在匹配关系，可以直接ICP出最终结果<br>

-----
ORB用EPnP+RANSAC进行几何校验<br>
问题1：<br>
为什么EPnP 需要有3个






### Sim3 群
未完待续


### SAD 匹配
reference: https://blog.csdn.net/hitzijiyingcai/article/details/83178348?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.baidujs&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.baidujs

一个滑动匹配的过程
