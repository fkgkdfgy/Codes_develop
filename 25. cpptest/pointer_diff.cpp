/*
 * @Author: Liu Weilong
 * @Date: 2021-01-19 20:11:34
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-01-19 20:19:23
 * @Description:
 *               测试主要内容：
 *               1. max_element   发现
 *               2. 指针相减
 */
#include <iostream>
#include <vector>
#include <cmath>


using namespace std;

int main()
{
    vector<double> temp;
    for(double i=0.0;i<10.0;i++)
    {
        temp.push_back(i);
    }

    double * ptr = temp.data();
    cout<<"the begin is "<< *ptr<<endl;
    cout<<"the end is "<<*(ptr+9)<<endl;
    cout<<"the vector end is "<<endl;
    
    cout<<"the diff is "<< (ptr+8) -ptr<<endl;

}