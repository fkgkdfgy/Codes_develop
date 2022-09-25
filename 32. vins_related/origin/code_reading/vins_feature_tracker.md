<!--
 * @Author: Liu Weilong
 * @Date: 2021-02-23 09:29:42
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-02-23 13:26:41
 * @FilePath: /3rd-test-learning/32. vins_related/origin/code_reading/vins_feature_tracker.md
 * @Description: 
-->
### VINS_feature_tracker
主要内容来自  feature tracker
1. readImage() 是函数主体

===========
理论的部分就是光流
不论是VINS的GFTT 和 opticalflow 都可以进行加速

===========

1. readImage()<br>
a.1. 前后图像的EQUALIZED
a.2. 