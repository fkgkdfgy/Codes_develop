/*
 * @Author: Liu Weilong
 * @Date: 2020-09-07 00:43:19
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-09-07 08:27:47
 * @Description: 用于测试gtest 的include 和lib 是否链接正确
 */


#include <iostream>
#include <numeric>
#include <gtest/gtest.h>

using namespace std;


// 这里的名字和cmake当中的名字不用相同
TEST(gtest_1, Sum)
{
    std::vector<int> vec{1, 2, 3, 4, 5};
    int sum = std::accumulate(vec.begin(), vec.end(), 0);
    EXPECT_EQ(sum, 15);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv); // 用于gtest 的一些命令行初始化
    return RUN_ALL_TESTS(); // 运行所有的测试样例
}