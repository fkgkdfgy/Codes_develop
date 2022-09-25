<!--
 * @Author: Liu Weilong
 * @Date: 2021-02-01 10:34:45
 * @LastE 是ditors: Liu Weilong 
 * @LastEditTime: 2021-03-26 17:10:48
 * @FilePath: /3rd-test-learning/32. vins_related/pl-vio/doc/Process.md
 * @Description: 
-->
### 编译
1. opencv 需要3.4 以下的版本 以为 LSD 在3.4 之后已经不支持了

### 内容
1. LSD + LBD 
2. Check 的部分只有 新旧disparity 的check、match distance <30    
3. NearLineTracking 检查  线中点的pixel 边缘的check，还有一个角度的check
