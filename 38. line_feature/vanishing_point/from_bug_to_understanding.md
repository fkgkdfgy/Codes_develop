<!--
 * @Author: Liu Weilong
 * @Date: 2021-04-12 16:10:29
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-04-12 16:29:44
 * @FilePath: /3rd-test-learning/38. line_feature/vanishing_point/from_bug_to_understanding.md
 * @Description: 
-->
### 梗概
1. 从BUG当中修正自己对于问题的理解

### 主要内容
1. 从赋值方法错误学习 $R_c^w$



-----
1. 从赋值方法错误学习 $R_c^w$
   问题描述:
   在添加$R_c^w$的旋转预估之后，出现巨大抖动的问题，这里目前怀疑是 VP 的旋转估计抖动性带来的，这一点会破坏VO的跟踪连续性。






