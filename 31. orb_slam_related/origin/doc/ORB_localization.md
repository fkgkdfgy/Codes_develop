<!--
 * @Author: Liu Weilong
 * @Date: 2021-06-16 12:52:56
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-16 13:10:30
 * @Description: 
-->
### OBR Localization 
这里主要是理一下ORB 的定位思路。

内部主要是由mbOnlyTracking确定是否是定位模式

由mbVO 确定是使用TrackWithMotionModel 还是使用 Relocalization

1. 一开始进行relocalization 补充MapPoint
2. 在MapPoint 跟踪相对充足的时候(>10) mbVO = false，这个地方的思路有点反，mbVO=false代表不需要重定位来补充MapPoint
3. 在MapPoint 跟踪不足的时候(<20)，使用Relocalization来补充MapPoint 
4. 在进行完 2. 或者3. 之后，如果mbVO = false，进行trackLocalMap，来补充MapPoint
5. 然后后面继续进行插入关键帧的操作。但是这里mbOnlyTracking会把NeedNewKeyframe直接返回一个false，也就是说localmapping不会进行运转。

备注: 说实话这么一套运转下来，定位起作用的只有已经生成的map. TrackWithMotionModel里面的UpdateLastFrame，并没有创造新的temp MapPoint。 我的天