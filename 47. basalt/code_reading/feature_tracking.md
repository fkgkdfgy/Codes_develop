<!--
 * @Author: Liu Weilong
 * @Date: 2021-05-10 08:56:44
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-29 22:11:48
 * @FilePath: /Codes/47. basalt/code/feature_tracking.md
 * @Description: 
-->
1. 入口 feed_images()
2. processingLoop->processFrame 函数主体
    |__ 通过 t_ns 判断是否是第一帧
        |__ pyramid       构造金字塔
        |__ addPoints     
            |__ detectKeypoints   按cell 均匀提取特征
            |__ 给每一个角点添加 SE2上的仿射           感觉精度上应该还是不如SVO,但是对于多相机之间的匹配应该是够了
            |__ 如果有多个相机，进行多相机的特征匹配
                |__ trackPoints()       放入new_pose0 输出 new_pose1
                    |__ 左右循环匹配， SOFT 的思想  如果距离差的太远就拒绝掉这个点
        |__ filterPoints
            |__ 使用双目外参，进行极线约束的匹配 来进一步对KeyPoint 进行检验
    |__ 不是第一帧
        |__ pyramid
        |__ trackNewAndOld
        |__ addPoints
        |__ filterPoints
    没有使用IMU信息啊，感觉不太行，可能唯一的trick 就是一个se2 + 循环验证 + 外参极线验证


注: basalt 的确速度可以做到非常快，里面所有的帧都在进行优化，那么basalt精度高的确是有一点道理的，毕竟所有的都进行了优化。