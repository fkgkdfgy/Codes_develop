<!--
 * @Author: Liu Weilong
 * @Date: 2021-01-16 11:11:18
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-21 13:50:50
 * @FilePath: /3rd-test-learning/22. slam_demo/slam_basic_and_environment/doc/doc.md
 * @Description: 
-->
### Eigen EigenSolver

SelfAdjointEigenSolver 给对阵矩阵使用的EigenSolver<br>
用于求解 EpipolarH4Pts $A^{T}Ah=0$ <br>
但是对于 H 矩阵的求解, 有要求 4个点 不能共线, 不能交叉。<br>

### 两种
