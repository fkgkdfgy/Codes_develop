<!--
 * @Author: Liu Weilong
 * @Date: 2021-05-06 18:55:13
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-05-06 19:12:25
 * @FilePath: /Codes/45. self_backend/ice_ba_reading/class.md
 * @Description: 
-->
### 类解析
Frame
    |__ member var
        |__ Tag T    一个Frame 的标签，包含 id ，timestamp ， 左右双目对应的照片名 
                     以及一系列 save load 大小比较的操作
        |__ 

Camera 
    |__ member var
        |__ 

KeyFrame 
    |__ member var
        |__ Xs       vector MapPoint

MapPoint
    |__ Point3D X    内部包含点3d位置和global point id
    |__ std::vector MapPointMeasurement zs; 

MapPointMeasurement
    |__ iFrm         frame id
    |__ idx          global point ID
    |__ Point2D  x   对应的观测
    |__ ubyte  right 是否是右目观测







