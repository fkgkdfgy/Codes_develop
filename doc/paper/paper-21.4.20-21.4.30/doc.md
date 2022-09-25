<!--
 * @Author: Liu Weilong
 * @Date: 2021-04-21 18:04:50
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-07-08 16:43:12
 * @FilePath: /Codes/doc/paper/paper-21.4.20-21.4.30/doc.md
 * @Description: 
-->
1. Active Exposure Control for Robust Visual Odometry in
HDR Environments

内容:
a. 光度标定 f photometric reponse 函数的逆函数 $In f^{-1}$
![](./pic/5.png)
b. 进行光照控制 通过之前定义的四种代价项，对$\Delta t$求导
得到最佳$\Delta t$
c. 光照补偿

直接法

![](./pic/1.png)
![](./pic/2.png)

光流

![](./pic/3.png)
![](./pic/4.png)

距离再好一点的光流，还差一个patch 的仿射

d. 其余参数调节，见论文 Implement Detail

![](./pic/6.png)