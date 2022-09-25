<!--
 * @Author: Liu Weilong
 * @Date: 2021-03-15 16:25:20
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-15 16:26:34
 * @FilePath: /3rd-test-learning/31. orb_slam_related/YGZ/doc/remain_task.md
 * @Description: 
-->
### 剩余的问题
1. DirectMethod一开始没有跑起来的原因？

   目前猜测的原因是Cached 内部的点比较少，总是失败跟踪，所以一开始只能用原版ORB 的代码