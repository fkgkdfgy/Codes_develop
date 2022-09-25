<!--
 * @Author: Liu Weilong
 * @Date: 2021-05-02 15:24:47
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-02 15:35:34
 * @Description: 
-->
### 2021.5.2 ceres 内部进一步精细化阅读
代码阅读之后，进行自身后端框架搭建
针对 curve_fitting.cc 进行代码解析

problem 和 problemImpl 分离式的实现

problem
    |__ AddResidualBlock           内部调用 problem_impl_->AddResidualBlock
        |__ problem_impl_->AddResidualBlock
            |__ check parameter的重复添加
            |__ 

2. CostFunction
    |__ member var
        |__ parameter_block_sizes_  vector<int32>   存储优化参数parameter的Sizes
        |__ num_residuals_                          存储residual 的Size
    |__ Evaluate()                                  计算residual 和 jacobian
    |__ parameter_block_sizes()                     return const parameter_block_sizes_
    |__ num_residuals()                             return num_residuals_
    |__ set_num_residuals()         
    |__ mutable_parameter_block_sizes               return parameter_block_sizes_







