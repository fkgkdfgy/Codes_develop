### 主要内容梗概

Bundle Adjustment — A Modern Synthesis 论文阅读

主要内容包括:
1. Cost Function 的选择和鲁棒性的选择?
2. 数值优化
3. 测量不确定性
4. 质量控制??

论文目的:
1. 纠正BA是比较慢的想法 -> 当考虑到矩阵的稀疏性会非常快
2. 纠正只需要线性代数 -> 收敛的算法非常复杂
3. 纠正所有的Sequence都可以使用 -> 对比Section11,好像这里的意思是之前认为进行BA之前必须设计一个网络??? 存疑，之后可以看一下
   
   gives some brief hints on network design, i.e. how to place your shots to ensure accurate, reliable reconstruction
   
4. 纠正点的重建是准确的