/*
 * @Author: Liu Weilong
 * @Date: 2020-09-07 08:32:30
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-09-07 09:01:27
 * @Description: 主要是为了测试 gtest 里面的宏 ASSERT 和 EXPECT
 */

#include <gtest/gtest.h>
#include <iostream>

int Add(int a, int b){
    return a+b;
};

void test()
{
    ASSERT_EQ(Add(2,4),5); 
    EXPECT_EQ(int(4),Add(2,3))<<"Aha there is something wrong _3!"; // 这一句不会显示，因为已经return了
};



int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc,argv);
    
    EXPECT_EQ(int(4),Add(2,3))<<"Aha there is something wrong !"; // 会继续运行， EXPECT 内部没有return 所以可以正常使用

    //ASSERT_EQ(Add(2,3),5);             // 这里失败的原因是 ASSERT_EQ 内部存在一个 return; 那么函数的返回应该对应 void 在main 内对应的是 int 所以会报错

    test();

    EXPECT_EQ(int(4),Add(2,3))<<"Aha there is something wrong _2!"; 

    return 0;
}

