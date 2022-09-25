<!--
 * @Author: Liu Weilong
 * @Date: 2021-03-02 09:56:27
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-02 10:48:49
 * @FilePath: /3rd-test-learning/28. vslam_course/HW6/algorithm.md
 * @Description: 
-->
### slambook2 ch9 代码流程梳理
主要内容:
1. LK SingleLayer\MultiLayer
2. Direct SingleLayer\MultiLayer


=======

具体内容：

1. LKSingleLayer(单个像素):<br>
   输入: 预测像素坐标,原图像像素坐标，2张图像(pre\cur) 输出: 优化后的像素坐标<br>
   a. 一个 GN 优化过程:<br>
   a.1. 初始化 H b J<br>
   a.2. Template error 计算<br>
   $$
    I_{pre}(x_p+dx,y_p + dy) - I_{cur}(x_c+dx,y_c+dy) ,(dx,dy)\in T
   $$
   a.3. 计算J<br>
   a.4. 叠加 H 和b<br>
   a.5. 出更新的值 更新$x_c,y_c$<br>
   a.6. H b 置零

2. LKMultiLayer(单个像素):<br>
   输入： 预测像素坐标，原图像坐标，2张图片(pre\cur) 输出: 优化后的像素图像<br>
   a. 对pre\cur两张图像，构建图像金字塔<br>
   b. 生成最上层(最小图)的坐标点<br>
   $$
    kp_{new} = kp_{origin} * scale
   $$
   c. 迭代主体:<br>
   c.1. 进行LKSingleLayer优化<br>
   c.2. 求得下一个是层级图像的像素点位置<br>
   $$
    kp_{n-1} = kp_n/scale
   $$
   c.3. 下一个层级的图像进行 c.1. <br>

3. DirectSingleLayer(单个点):<br>
   a. 计算 Error Jacobian<br>
   b. 计算 更新量<br>