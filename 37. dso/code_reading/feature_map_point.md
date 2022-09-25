<!--
 * @Author: Liu Weilong
 * @Date: 2021-04-26 16:09:25
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-04-27 11:02:42
 * @FilePath: /Codes/37. dso/code_reading/feature_map_point.md
 * @Description:
-->
### 主要内容
1. Frame
2. Feature
3. Map

---- 
1. Frame--FeatrueShell
   FrameShell.h    Frame 内壳
    |__ member var
	    |__ int id; 			           INTERNAL ID, starting at zero.
	    |__ int incoming_id;	           ID passed into DSO
	    |__ double timestamp;		       timestamp passed into DSO.
        	// set once after tracking
        |__ SE3 camToTrackingRef;            
        |__ FrameShell* trackingRef;
        	// constantly adapted.
        |__ SE3 camToWorld;				   Write: TRACKING, while frame is still fresh; MAPPING: only when locked [shellPoseMut
        |__ AffLight aff_g2l;              光照仿射
        |__ bool poseValid;                存疑

2. Frame--EFFrame   wrapper for frame hessian, contains the priors 内部内容使用存疑
3. Frame--FrameHessian
   |__ member var
       |__ dIp                 存储多层金字塔信息  灰度、dx、dy 对应Vector3f 0 1 2
       |__ absSquaredGrad      存储多层进行塔 像素梯度abs 
   |__ makeImages()            根据原图像创建金字塔
       |__ 1. 为多层金字塔的存储创造空间
              dIp、absSquaredGard 
           1. dIp、absSquaredGrad 进行信息填充
   