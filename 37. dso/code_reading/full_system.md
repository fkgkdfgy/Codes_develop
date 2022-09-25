<!--
 * @Author: Liu Weilong
 * @Date: 2021-04-26 16:04:27
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-04-27 11:03:46
 * @FilePath: /Codes/37. dso/code_reading/full_system.md
 * @Description: 
-->
### 主体分析
1. addActiveFrame            调用主体函数


--------
1. addActiveFrame
   |__ add into allFrameHistory    创建新的FrameShell 和 FrameHessian 并将新的Frame Shell 放入 allFrameHistory
   |__ makeImages()
   |__ if(!init)
       |__ firstFrame: setFirst                