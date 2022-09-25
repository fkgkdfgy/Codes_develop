<!--
 * @Author: Liu Weilong
 * @Date: 2021-04-15 10:07:26
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-04-15 10:15:24
 * @FilePath: /Codes/31. orb_slam_related/origin/doc/ORB_assessment.md
 * @Description: 
-->
### ORB的总体评价
1. ORB是一套偏向观测的框架，也就是说不存在信息的preprogate 只有measurement, 足够的数量的measurement就可构成一个系统。也因为是一个偏向观测的系统也就是说是一套MLE的系统，而不是MAP的系统，在没有系统先验的情况下，对于方差的使用，就仅仅局限在了对于观测模型噪声的建模上，而且这部分的建模也是比较粗糙的。
   例子，所有的ORB 优化都是在
   $$
   (MLE) \argmax_x p(z|x) = f(x,n)
   $$
   对于x本身引入的噪声建模是完全没有进行考虑

2. ORB 对于细节的考虑非常的多，这也直接导致整个系统非常的重