<!--
 * @Author: Liu Weilong
 * @Date: 2021-01-16 22:12:11
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-01-30 07:19:52
 * @Description: 
-->
### 1. 熟悉 Eigen 矩阵运算
1. 在什么条件下，x 有解且唯⼀?<br>
   A为列满秩<br>
2. ⾼斯消元法的原理是什么?<br> 
   一种求逆的手段   $A\rightarrow{LDU}$ 分解 <br>
   $L\in{R^{m×m}}$ 是一个下三角矩阵 $D\in{R^{m×m}}$ 是对角阵 $D\in{R^{m×n}}$ 是一个上三角矩阵<br>
   把左边的矩阵通过行运算变为单位阵,左边矩阵的就会出现在右边<br>
   用于加速计算<br>
   i.e.
   $$
        \left[
        \begin{array}{cc|c}
        1 & 2 &1&0\\
        4 & 5 &0&1
        \end{array}
        \right]
        \rightarrow
        \left[
        \begin{array}{cc|c}
        1 & 0 & \frac{-5}{3} & \frac{2}{3}\\
        0 & 1 & \frac{4}{3} & \frac{1}{-3}
        \end{array}
        \right]
   $$
3. QR分解的原理?<br>
每一实方阵，都可以分解成一个正交阵和一个上三角矩阵
4. Cholesky分解的原理？<br>
一个对阵正定阵可以分解成一个$LL^{T}$ 一个下三角矩阵和这个下三角矩阵的转置的乘积。
5. 编程实现见matrix_calculation.cc

### 2. 几何运算练习 
见coordinate_calculate.cpp

### 3. 旋转的表达
1. 设有旋转矩阵 R，证明 RT R = I 且 det R = 1<br>
证明正交 $R^TR=I$<br>
$$
\begin{aligned}
   R^TR &= ([e_1,e_2,e_3]^{T},[e_1^{'},e_2^{'},e_3^{'}])^{T}([e_1,e_2,e_3]^{T},[e_1^{'},e_2^{'},e_3^{'}])
   \\ &= [e_1^{'},e_2^{'},e_3^{'}]^{T}[e_1,e_2,e_3]([e_1,e_2,e_3]^{T},[e_1^{'},e_2^{'},e_3^{'}])
   \\ & =I
\end{aligned}
$$
证明行列式 $det(R)=1$<br>
$$
   det(R^{T}R)=1 \rightarrow
    det(R) = \plusmn1
$$
所以，只要证明 $det(R)\not ={-1}$ 就可以了<br>
举一个例子<br>
$I\in{R}$<br>
$det(I)\not ={0}$<br>
所以$det(R)\not ={-1}$


2. 设有四元数 q，我们把虚部记为 $\epsilon$，实部记为 η，那么 q = ($\epsilon$; η)。请说明 $\epsilon$和 η 的维度。<br>
虚部3维,实部1维
3. 证明四元数乘法<br>
从课件，拆分之后即可得到这个答案
$$
\begin{aligned}
   q_aq_b&=[s_as_b-v_a^{T}v_b,s_av_a+s_bv_b+v_a×v_b]
   \\&= q_1^{+}q_2
\end{aligned}
$$
另外一个 只要将
$$
v_a×v_b=-1v_b×v_a
$$
拆分后就可以得到
### 4. 罗德里格斯公式证明


### 5. 四元数性质验证
验证点旋转
$$
   qpq^{-1}=q^{+}q^{-1\bigoplus}p = 
   \left[
      \begin{matrix}
      (ηI+\epsilon×)(ηI+\epsilon×)+\epsilon\epsilon^{T} &0\\
      0 &\epsilon^{T}\epsilon +η^{2}
      \end{matrix}
   \right]
   \left[
      \begin{matrix}
         p\\
         0
      \end{matrix}
   \right]
$$
所以p2 的实部也一定是0
### 6. 熟悉C++
使用的C++11 的内容有，
1. for auto 的遍历， 替代了之前的for(;;)
2. auto 进行自动类型的推导
3. lamda 匿名函数
4. avec{a1,a2,a3} uniform initialization