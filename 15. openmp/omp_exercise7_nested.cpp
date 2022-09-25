/*
 * @Author: Liu Weilong
 * @Date: 2020-10-26 14:47:52
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-10-26 15:05:18
 * @FilePath: /3rd-test-learning/17. openmp/omp_exercise7_nested.cpp
 * @Description: 针对编程当中的 nested_parallel 进行编程 
 *               new API:
 *               #pragma omp parallel for collpase
 *               
 *               两种错误：
 * 1.            #pragma omp parallel for
 *               for 第一层
 *                   #pragma omp parallel for   这一句编译的时候会被忽略
 *                   for 第二层
 * 2.            #pragma omp parallel for
 *               for 第一层
 *                   #pragma omp for   这一句编译的时候会直接报错
 *                   for 第二层
 */

#pragma omp parallel for collapse(2)
for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 6; ++j) {
        c(i, j);
    }
}