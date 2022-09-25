/*
 * @Author: Liu Weilong
 * @Date: 2021-01-21 07:51:02
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-01-21 07:53:01
 * @Description: 
 * 
 *              内存分配测试
 */

#include <iostream>

using namespace std;

int main()
{
    int i =6;
    const int j = i;
    int * i_ptr = new int[i]();
    int * j_ptr = new int[j]();
}