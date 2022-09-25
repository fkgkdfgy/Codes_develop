<!--
 * @Author: Liu Weilong
 * @Date: 2021-04-27 11:03:57
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-04-27 11:10:19
 * @FilePath: /Codes/37. dso/code_reading/Initializer.md
 * @Description: 
-->
### 主要内容来自
1. CoarseInitializer


-----
1. CoarseInitializer--Member Var
   |__ K w h fx fy cx cy 对应相机内参、width、height、具体内参系数 (存储金字塔信息)
   |__ Ki fxi fyi cxi cyi 对应相机内参的逆、以及各个具体内参系数位置上的K逆的值  (存储金字塔信息)
   |__ firstFrame
2. CoarseInitializer--setFirst()
   |__ for each level of Pyr
       |__ 
3. CoarseInitializer--makeK()
   |__ 生成多层金字塔的内参K