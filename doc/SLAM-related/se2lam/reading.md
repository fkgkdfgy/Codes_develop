<!--
 * @Author: Liu Weilong
 * @Date: 2021-01-04 21:36:44
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-01-06 08:05:14
 * @Description: 
-->

#### test_vn 主函数

system
    |__
    |__ interface
        |__ receiveOdoData
        |__ receiveImgData
    
Track
    |__
    |__ mRun      主函数入口
        |__ mReadData      读取数据
        |__ mCreateFrame   第一帧的时候调用
        |__ mTrack         非第一帧的时候调用
            |__ mMatchByWindow 一个简单的搜索,并没有Project等操作
            |__ mRemoveOutliers       F矩阵+RANSAC去outliers 
    |__ mResetLocalTrack   重置函数 重置坐标\更改RefFrame等

LocalMapper
    主要会有一次LocalMap 的加载
    一个LocalMap 的 Keyframe 去冗余
    一个OutlierRemove