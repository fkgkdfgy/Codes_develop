/*
 * @Author: Liu Weilong
 * @Date: 2020-09-21 13:37:28
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-01-15 08:25:56
 * @FilePath: /3rd-test-learning/5. gtest/gtest_test_f.cpp
 * @Description: 专门用于学习和测试 TEST_F 的文件
 *               主要内容包括：1. gtest_f 使用的类是否只创建了一次 结论是每一次 TEST_F 都会进行一次ctor 和dtor
 *               
 *               例如:下面的例子中
 *                实验结果就是每一次都会创造一个新的Atest 对象
 *                count_ 也会完全重置 而不是保存数据给下一次进行测试
 */

#include "gtest/gtest.h"
#include <iostream>
#include <vector>
using namespace std;

class A
{
    public:

    A(int tmp):content_(tmp){cout<<" the ctor of A is called"<<endl;}
    ~A(){cout<<" the dtor of A is called "<<endl;}

    template <typename FirstType,typename ... TypePack>
    void print (FirstType firstType, TypePack ... typePack)
    {
        cout<<firstType<<endl;
        cout<<"the quantity of left param is "<<sizeof...(typePack)<<endl;
        print(typePack...);
    }
    
    template <typename Type>
    void print(Type && tmp)
    {
        cout<<tmp<<endl;
        cout<<"the content is "<<10<<endl;
    }

    int content_;
    
};

class ATest:public testing::Test
{
    public:

    ATest():beTestedA(10){cout<<"ctor is used "<<endl;} 
    
    static void SetUpTestCase(){cout<<" Used before a cluster of test cases"<<endl;}

    virtual void SetUp(){cout<<"do something!"<<endl; } 

    virtual void TearDown(){cout<<"do something after this test"<<endl;count_++;}

    static void TearDownTestCase(){cout<<"Used after s cluster of test cases"<<endl;}
    
    ~ATest(){cout<<"dtor is used "<<endl;} 

    A beTestedA;
    int count_ =0 ;
};
// 实验结果就是每一次都会创造一个新的Atest 对象
// count_ 也会完全重置 而不是保存数据给下一次进行测试
TEST_F(ATest,printTest)
{
    beTestedA.print(10.0,"aaaa",123,"sssss");
    beTestedA.content_++;
    cout<<"now the count_ is "<<count_<<endl;
}

TEST_F(ATest,contentTest)
{
    EXPECT_EQ(beTestedA.content_,10);
    beTestedA.content_ = 10;
    cout<<"now the count_ is "<<count_<<endl;
}


