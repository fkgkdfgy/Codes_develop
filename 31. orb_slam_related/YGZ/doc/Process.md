<!--
 * @Author: Liu Weilong
 * @Date: 2021-02-02 10:30:22
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-02-02 14:17:38
 * @FilePath: /3rd-test-learning/31. orb_slam_related/YGZ/doc/Process.md
 * @Description: 
-->
运行过程：\
1. ./generate.sh
2. error: 
   ![](../picture/1.png)
   CMakeLists.txt 改为
   find_package(glog REQUIRED)
   link_libraries(glog)
3. glog 和gflags 需要使用 .so 的安装
4. glog 和gflags 的配置被我搞乱了，\
   去掉代码当中的 glog 之后顺利打开\
   速度4x左右,而且稳定
   