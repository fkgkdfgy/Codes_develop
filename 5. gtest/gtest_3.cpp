/*
 * @Author: Liu Weilong
 * @Date: 2020-09-07 09:17:08
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-09-07 10:16:07
 * @Description: gtest 事件机制学习，TEST_F() 的使用， _F == _fixture 用于共享数据配置
 */


#include <gtest/gtest.h>
#include <iostream>

using namespace std;

// TestCase 机制 先ctor->SetUp()->TearDown()->dtor
class A: public testing::Test
{
    public:
    A():a_(0.05){ cout<<"ctor is using!"<<endl;}
    void SetUp(){cout<<"AAAA"<<endl;}
    void TearDown() {cout<<"BBBB"<<endl;}
    ~A(){cout<<"dtor is using !"<<endl;}
    int a_;
};

TEST_F(A,isa_)
{
    EXPECT_EQ(a_,0.05);
    ASSERT_EQ(a_,0.01);
}

// TestSuiteCase 机制
class B:public testing::Test
{
    public:
    static void SetUpTestCase(){cout<<" Used before a cluster of test cases"<<endl;}

    static void TearDownTestCase(){cout<<"Used after s cluster of test cases"<<endl;}

    static int a_;
};
int B::a_ = 0;

TEST_F(B,test_1)
{
    EXPECT_EQ(a_,0);
    a_++;
}
TEST_F(B,test_2)
{
    EXPECT_EQ(a_,3);
    a_++;
}

TEST_F(B,test_3)
{
    EXPECT_EQ(a_,2);
    a_++;
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc,argv);

    return RUN_ALL_TESTS();
}