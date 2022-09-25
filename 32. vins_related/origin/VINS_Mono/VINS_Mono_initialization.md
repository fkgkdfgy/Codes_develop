<!--
 * @Author: Liu Weilong
 * @Date: 2021-03-22 19:54:22
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-05-08 16:31:23
 * @Description: 
-->
### VINS IMU 初始化 Understand Why？
1. 找到IMU 和 Camera 外参 
   为什么外参只有旋转没有位移？
   在旋转一致的情况下，刚体相对位移保持一致，无法进行估计。
   E矩阵求解旋转
2. 寻找稳定的跟踪点  基础:没有跟踪谈什么IMU和Camera初始化
3. PnP求解旋转 没有位姿怎么和IMU融合 (相机的Rotation是IMU 的真值)
4. Rotation 用于 IMU 旋转的bias 标定 因为在单目的情况下PnP得出的旋转是准的(位移不准)，所以可以使用Rotation对IMU进行标定
5. 使用IMU的加速度信息进行速度、重力大小、尺度上的优化
   为什么没有优化相机位移？ 因为只差了一个尺度大小，所以优化尺度就相当于优化了相机位移。
   为什需要优化重力？
   因为大小不一定是9.81
   (IMU是Camera 的真值)
6. 矫正重力方向
   为什么需要矫正重力方向？因为IMU的安装一定不是完全垂直的
   所以需要加上方向偏执重新继续优化
   (重力方向真的有什么是可以进行估计的吗？这个真的可以吗？)
7. 进行相机坐标系的对齐

### 遗留问题
1. 为什么没有关于平移bias 的估计呢？
2. 如果判断重力方向是有效的优化？
需要VINS on Wheel 的方法来进行分析了。

### 位姿估计
2d -2d : 估计 EH 从 E 和 H 之中,进行位姿拆解，但是这里有一个尺度
3d -2d : PnP
3d -3d : ICP

### 外参估计公式
$$
   R_{imu}^{cam_{j}} = R_{imu}^{cam_{i}}(外参)R_{cam_i}^{cam_j} = R_{imu_{i}}^{imu_j}R_{imu_j}^{cam}（外参）
$$
### 原理性的分析
主要内容来自
1. CalibrationExRotation
2. initialStructure

-----
1. CalibrationExRotation  主要是学习一下Eigen 求解超定方程的方法
1.a. 得到两帧之间的对应的特征点，然后对特征点进行旋转求解
     a.1. 求旋转使用solveRelativeR 内部是求解E(暗示feature 是2d-2d)，然后从E分解R，验证R
     a.2. CalibrationExRotation 添加数据 10 帧数据之后的结果，确认可信
          内部使用SVD 求解一个Ax=0 内部存在huber(5.0)的一个huber核
          这里使用四元数进行表示
$$
   R_{imu}^{cam_{j}} = R_{imu}^{cam_{i}}(外参)R_{cam_i}^{cam_j} = R_{imu_{i}}^{imu_j}R_{imu_j}^{cam}（外参）
   \\
   \rightarrow  q_{imu}^{cam_{i}}(外参)q_{cam_i}^{cam_j} = q_{imu_{i}}^{imu_j}q_{imu_j}^{cam}
   \\
   q_i^{ci}\times q_{ci}^{cj} = q_{ii}^{ij}\times q^c_i
   \\
      q_i^{ci}\times q_{ci}^{cj} - q_{ii}^{ij}\times q^c_i =0
   \\
   Final:\\
   huber*[L(q_{ci}^{cj}) - R(q_{ii}^{ij})]q^c_i = 0
$$
最终，公式使用SVD 就可以进行求解。
然后对第三个特征进行检验，查看是不是过小，如果过小也就是说明，A的秩为2 而不是3 求出来的解不靠谱。
其实这里应该使用比例性的检验，而不是一个单纯的比较阈值。

2. initialStructure
2.a. 得出帧间的的旋转给IMU 旋转Bias 当作真值使用
2.b. solveGyroscopeBias 求解 Gyro 的旋转Bias



-----
### 细节分析
1. E 的分解和验证
   E 分解之后有四种情况，只有第一种情况可以让大量特征点在相机的前方
2. SVD的求解
   $$
      Ax = 0\\
      A = USV^T\\
      A^TA = VS S^TV^T\\
   $$
   这里存在一个反向思考的问题
   $$
      AX = X\Lambda
      \\
      A = X\Lambda X^{-1} 
   $$
   所以实际上作用在A上的是X的列 而不是行，也就是为什么SVD 列才是目的,解决的问题的是
   $$
      A^TAx =0
   $$
   
   V的列对应x的解
   关于求解可靠性，是依靠奇异值正常(不过小)的数量是不是和 正常A矩阵的秩相同来进行判断。
3. opencv 自身就有cv2eigen 或者eigen2cv 的转化？

### 代码分析
1. 外参标定
2. IMU+Camera初始化

---
1. 外参标定
   |__ f_manager.getCorresponding     得到两帧关联的像素点
   |__ CalibrationExRotation()        
       |__ 1. 从关联像素求解和保存相机旋转、保存IMU旋转、保存从IMU旋转推得的Camera旋转
              这里的Camera旋转求解只是使用了E ，没有使用H ，所以有时候旋转外参不准也是有可能的
       |__ 2. 凑够10个旋转，就开始认为求解的旋转靠谱
       |__ 3. 如果SVD求得的特征值，只有一个非常小，一般认为外参靠谱(也就是有两个绕不同轴的旋转，就可以进行一个完整的旋转外参初始化)
2. IMU+Camera 初始化 initialStructure()
   |__ 1. 通过重力方向的方差(sum_g tmp_g) 来判断IMU 的旋转量是否是已经达到要求,和SVD的特征值来看目的基本类似
   |__ 2. 

   不再进行赘述，过程和最开始的笔记基本一致