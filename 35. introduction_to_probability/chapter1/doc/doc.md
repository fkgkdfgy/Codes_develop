<!--
 * @Author: Liu Weilong
 * @Date: 2021-02-07 14:09:32
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-02-19 12:22:05
 * @FilePath: /3rd-test-learning/35. introduction_to_probability/chapter1/doc/doc.md
 * @Description: 
-->
### Chapter1 
1. definition
   Sample Space and event
2. Naive definition of probability
   $$
    P_{naive}= \frac{|A|}{|S|}=\frac{number of outcomes favorable to A}{total number of outcomes in S}

   $$
3. 从2 可以得出，最主要的得到概率的问题是如何数可能性的问题 -> 数数的方法\
3.a. Multiplication rule a个subset 然后每个有b种可能，一共 ab 种可能。\
这里直接涉及到，有顺序和无顺序的问题\
还有就是 subset 有重叠和无重叠的问题\
可以先考虑有顺序,然后再考虑无顺序会简单很多\
3.b. Sampling with/without replacement\
i.e. Birthday Problem  5个人里有两个人同一天生日\
$$
    P_{naive} = \frac{C^5_2*365*364*363*362}{365^5}
$$
tips: 解决的方法就是对对象进行标注，把每个当作不同的个体进行计数。
i.e. Leibniz's mistake\
两个六面筛子，得到11 和 12 的概率有多少\
$$
    P_{naive} = \frac{3}{6^2}
$$

4. Adjusting for overcounting  用于调整重复计数的问题 -> Binomial coefficient 统计 n个对象里面，有多少个set\
$$
\left(
    \begin{matrix}
        n\\k
    \end{matrix}
    \right) = \frac{n(n-1)(n-k+1)}{k!}=\frac{n!}{(n-k)!k!}

$$
i.e. Permutation Letter LLLAAAAA 有多少种排列方法\
$$
    \left(
        \begin{matrix}
            8\\3
        \end{matrix}
        \right)    
$$
i.e. Permutation STATISTICS\
$$
    \left(
        \begin{matrix}
            10\\3
        \end{matrix}
        \right)
         \left(
        \begin{matrix}
            7\\3
        \end{matrix}
        \right)
         \left(
        \begin{matrix}
            4\\2
        \end{matrix}
        \right)
         \left(
        \begin{matrix}
            2\\1
        \end{matrix}
        \right)
$$
但是需要注意的是 现在还只是数数的阶段

i.e. Binomial Theorem\
$$
    (x+y)^n=\sum^n_{k=0}         
    \left(
        \begin{matrix}
            n\\k
        \end{matrix}
        \right) x^ky^(n-k)
$$
证明:\
拆开$(x+y)^n$

i.e. full house in poker \
$$
    P_{naive}= \cfrac{14        \left(
        \begin{matrix}
            4\\3
        \end{matrix}
        \right)*13        
        \left(
        \begin{matrix}
            4\\2
        \end{matrix}
        \right)}{    
        \left(
        \begin{matrix}
            52\\5
        \end{matrix}
        \right)}
$$
i.e. Newton-Pepys problem\
$$
    P_{naive1} = 1-\cfrac{5^6}{6^6}\\
    P_{naive2} = 1-\cfrac{5^{12}}{6^{12}}-\cfrac{12*5^{11}}{6^{12}}\\
    P_{naive3} = 1-\cfrac{5^{18}}{6^{18}}-\cfrac{18*5^{17}}{6^{18}} - \cfrac{  \left(
        \begin{matrix}
            18\\2
        \end{matrix}
        \right)5^{16}}{6^{18}}\\
$$
i.e. Bose-Einstein 在n个有放回的object里面，取k次有多少种取法，忽略次序 -> isomorphic problem
答案是一共有
$$
\left(
        \begin{matrix}
            n+k-1\\k
        \end{matrix}
        \right)
$$
种。
tips：Bose-Einstein 并不合适naive definition of probability 因为各种情况，概率不均

5. Story proof 故事证明法\
   i.e. Choosing the complement
   $$
   \left(
        \begin{matrix}
            n\\k
        \end{matrix}
        \right)=
   \left(
        \begin{matrix}
            n\\n-k
        \end{matrix}
        \right)
   $$

   Commitee and teams 的故事\
    i.e. 

### <font color="Red">总结：如何数数</font>
1. 学会对问题对象进行标记，先有顺序，再无顺序，先无重复，再有重复
2. 朴素的概率
3. Multiple rule      a个subset  每个有b种情况，一共 多少种情况                    适合朴素的概率
4. Binomial Condinator 无顺序         不适合朴素的概率
5. Bose-Einsterin Problem 无顺序的方法 不适合朴素的概率