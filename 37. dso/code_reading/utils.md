<!--
 * @Author: Liu Weilong
 * @Date: 2021-04-26 15:40:30
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-04-26 16:03:56
 * @FilePath: /Codes/37. dso/code_reading/utils.md
 * @Description: 
-->
### DSO 大阅读基础
主要内容来自: src/utils
1. setting.h/.cpp      内部的各种设置
2. NumType.h
3. ImageAndExposure.h/.cpp
4. globalFuncs.h
5. FrameShell.h

6.  Eigen 的各种解释
-------
2. NumType.h  函数Eigen 的所有重命名
    |__ AffLight 
        |__ fromToVecExposure() 从四个参数 得到光度仿射 参数
        |__ vec()               获得Eigen 形式的光度仿射


3. ImageAndExposure.h/.cpp
   |__ ImageAndExposure
        |__ member var
            |__ float * image 图像irradiance ？ 这个是灰度吗？
	        |__ int w,h;				// width and height;
            |__ double timestamp;
	        |__ float exposure_time;	// exposure time in ms.
        |__ copyMetaTo()             曝光时间拷贝
        |__ getDeepCopy()            深度拷贝

4. globalFuncs.h
   |__ getInterpolatedElement()      插值
   |__ getInterpolatedElementXXXXX() 各种插值函数
   |__ 各种颜色的数值   

5. FrameShell.h    Frame 内壳
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

6.  Eigen 的各种解释
    EIGEN_ALWAYS_INLINE