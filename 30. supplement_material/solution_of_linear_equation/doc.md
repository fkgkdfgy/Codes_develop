<!--
 * @Author: Liu Weilong
 * @Date: 2021-01-25 13:19:02
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-19 17:34:31
 * @FilePath: /3rd-test-learning/30. supplement_material/solution_of_linear_equation/doc.md
 * @Description: 
-->
reference:https://zhuanlan.zhihu.com/p/54943042 LU分解<br>

### 总结
名字|条件|分解成果|复杂度
---|---|---|---
LU分解|方阵、可逆|一个下三角一个上三角|$\cfrac{2}{3}n^3$
Cholesky分解|对称、正定|一个下三角一个上三角，两个三角相同|$\cfrac{1}{3}n^3$
QR分解|m×n的有n个独立列的矩阵|a. 一个m×n 的正交阵+上三角<br>b. 一个m×m的正交方阵+非满秩上三角|$2mn^2-\cfrac{2}{3}n^3$(householder)
Schur补|任意矩阵|把一个大矩阵到求逆，分解成两个小矩阵的求逆|肯定没有Cholesky 分解快
SVD|任意矩阵|$U\Sigma V^T$ <br> U、V 是一个酋矩阵也就是说U^TU=I|$O(min(m^2n,n^2m))$

When to use Schur补？





----

1. LU 分解(高斯消元)<br>
   条件：<br>
   a. 方阵 b. 可逆 c.消元过程中没有0主元出现<br>
   作用：<br>
   加速 $Ax=b$计算<br>
   $$
    Ax=b\\
    LUx=b\\
    Ly=b\\
    Ux=y
   $$
   对比求逆的解法<br>
   ![](./picture/9.png)
   ![](./picture/10.png)
   变种：<br>
   $PA = LU$ 克服 c  条件 0主元的情况，需要对A进行一次换行 然后重新计算<br>
   
2. Cholesky分解<br>
   条件:<br>
   a. 正定 b. 对称 c. 方阵(隐含条件)<br>
   作用:<br>
   加速$Ax=b$计算<br>
   $$
   Ax=b\\
   LL^T=b\\
   Ly=b\\
   L^Tx=y
   $$
   时间复杂度:<br>
   ![](./picture/3.png)
   $O(\frac{1}{3}n^3)$


3. QR 分解<br>
   原理：<br>
   $A\in{R^{m×n}}$ A是有n个独立的列<br>
   这个条件决定了 A 的行数需要大于列数<br>
   $A = QR$\
   $Q\in{R^{m×n}} R\in{R^{n×n}}$

   作用:<br>
   加速$Ax=b$
   $$
   Ax = QRx =b\\
   Rx = Q^{T}b\\
   $$
   $$
   A = [q_1,q_2 ... q_n]R
   \\ ||q_i||=1
   \\ q_i^{T}q_j=0
   \\ q_i^Tq_i=1
   $$  
   ![](./picture/4.png)
   应用:<br>
   a. 伪逆和加速求逆<T>
   b. 空间投影<T>
   c. 线性方程计算<T>
   ![](./picture/5.png)
   ![](./picture/7.png)
   如何求:<br>
   a. Gram_schmidt<br>
   这里只用一部分举例
   $$

   A =    \left[
      \begin{matrix}
      a_1&a_2&a_3
      \end{matrix}
      \right] \in{R^{3×3}}\\
   \left[
      \begin{matrix}
      a_1&a_2&a_3
      \end{matrix}
      \right]=
   \left[
     \begin{matrix}
      q_1&q_2&q_3
      \end{matrix}
   \right]
      \left[
     \begin{matrix}
      R_{11}&R_{12}&R_{13}\\
      0&R_{22}&R_{23}\\
      0&0&R_{33}
      \end{matrix}
   \right]
   $$
   因为Q是正交阵。而且我们现在是已经知道结果推过程
   $$
   q_1R_{11}=a_1
   R_{11} = ||a_1||
   $$
   从公式可以看出
   $$
   a_2 = q_1R_{12}+q_2R_{22}
   $$
   $a_2$和$q_1$已知，$q_1$$q_2$ 正交，最后我们可以得到
   $$
      q_1^{T}a_2 = q_1^{T}(q_1R_{12}+q_2R_{22}) = R_{12}
      \\
      a_2-q_1R_{12} = q_2R_{22}
      \\
      ||a_2-q_1R_{12} || = R_{22}
   $$ 
   剩下的部分就是继续迭代
   b. HouseHolder

   ![](./picture/8.png)
   
   2021.1.31添加<br>
   在学习线特征的时候发现，
   其实也可以分解成
   $$
      A = Q_2R_2 = Q_1R_1\\
      Q_1\in{R^{m×n}} R_1\in{R^{n×n}}\\
      Q_2\in{R^{m×m}} R_1\in{R^{m×n}}\\
      Q_2 = \left[\begin{matrix}
      Q_1,t(Q_1)\\
      \end{matrix}
      \right]\\
      R_2 = \left[\begin{matrix}
      R_1\\
      \pmb{0}\in{R^{(m-n)×n}}
      \end{matrix}
      \right]
   $$

   t(Q_1)的意思就是做了一个填充。让 Q2 矩阵变成方阵且满秩

   这里做的第二种，相当于把 Q 从秩为n 的矩阵，补全成了 秩为m的矩阵
   

4. Schur Complement
   
   ![](./picture/6.png)
   过一次schur Complement 之后，进行进一步的加速

   <font color="Red"> 2021/7/31 补充 </font>

   关于什么时候使用Schur 补，而不是LLT(Cholesky分解)
   这个问题是在，想到Schur 和 Cholesky 到底什么时候用？
   
   目前认为的原因是上面Schur 补的图片有一些误导性。Schur面对的条件并不是对称阵。

   而是<font color = "Red">任意矩阵</font>

   真正对的理解应该是这样
   ![](./picture/12.png)



5. SVD 求解
   
   ![](./picture/11.png)

   SVD复杂度$O(min(m^2n,n^2m))$



















































































   
   