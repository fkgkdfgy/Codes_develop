<!--
 * @Author: Liu Weilong
 * @Date: 2021-03-02 14:16:33
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-02 14:34:36
 * @FilePath: /3rd-test-learning/39. bug_free/doc.md
 * @Description: 
-->
### 目的
1. 收集所有曾经犯过的错误，防止之后再次犯错，开始算法开发速度

### 内容
1. (来自LK_self_tracker.h) 应该放 cur_img 的位置放了 pre_img, 输入输出存在问题。
   -> 算法初始开发阶段，尽量减少传值函数的存在，减少输入输出错误。
2. (来自LK_self_tracker.h) GN 算法没有重置 cost
   -> 先开发框架，和检查框架