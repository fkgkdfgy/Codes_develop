<!--
 * @Author: Liu Weilong
 * @Date: 2021-03-29 08:56:00
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-31 13:33:04
 * @FilePath: /3rd-test-learning/38. line_feature/vanishing_point/2_line_exhaustive_searching_VP_detection_code_reading.md
 * @Description: 
-->
run()
    |__ getVPHypVia2Lines 2-Line MMS 对应的三个VPs 假设
        |__ 通过RANSAC 的参数进行迭代次数的预测
        |__ 使用输入的vector<vector<double>> 进行线的初始化
        |__ 得到VPs 假设
        |__ 只有这个部分需要进行修改
    |__ getSphereGrids 计算线和线的交点给积分球赋值 n^2 种情况
    |__ getBestVpsHyp 计算最大得分
    |__ line2vp
        |__ 

tips: VPs 有一个从Sphere到 ImagePlane 上的转换  也就是成上K 和不乘K
      