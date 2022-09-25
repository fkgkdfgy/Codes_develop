/*
 * @Author: Liu Weilong
 * @Date: 2021-06-29 20:55:04
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-29 21:18:05
 * @Description: 
 * 
 * 惊叹于 BASALT的代码质量  
 * 迅速地学习一下 assert 是如何编写的
 * 
 * __PRETTY_FUNCTION__  所在的function 标签   GCC 的拓展
 * __FILE__             所在的文件      标签
 * __LINE__             代码所在的行数   标签
 * __DATE__             在源文件中插入当前的编译日期
 * __TIME__             在源文件中插入当前编译时间；
 * 
 * 宏里面的#expr         可以转换成 const char * 然后进行打印
 * 
 * define 中的三个特殊符号 ##   #@    #
 * 
 * ##
 * x连接y，例如：int n = Conn(123,456); 
 * 结果就是n=123456;
 * char* str = Conn("asdf", "adf"); 
 * 结果就是 str = "asdfadf";
 * define Conn(x,y) x##y
 * 
 * #@
 * 给x加上单引号，结果返回是一个const char。
 * 例如：char a = ToChar(1);结果就是a='1';
 * 做个越界试验char a = ToChar(123);
 * 结果就错了;但是如果你的参数超过四个字符，编译器就给给你报错了！
 * error C2015: too many characters in constant   ：P
 * define ToChar(x) #@x
 * 
 * #
 * x加双引号,例如：char* str = ToString(123132);
 * 就成了str="123132";
 * #define ToString(x) #x
 * 
 * 
 */

#include <iostream>

using namespace std;

inline void assertion_failed(char const* expr, char const* function,
                             char const* file, long line) {
  std::cerr << "***** Assertion (" << expr << ") failed in " << function
            << ":\n"
            << file << ':' << line << ":" << std::endl;
  std::abort();
}

#define SELF_LIKELY(x) __builtin_expect(x, 1)

#define SELF_ASSERT(expr)                                               \
  (SELF_LIKELY(!!(expr))                                                \ 
       ? ((void)0)      /* 这里必须返回一个 void 的模型，因为三目运算不能有可能返回两个类型的结果*/                                                  \
       : assertion_failed(#expr, __PRETTY_FUNCTION__, __FILE__, \
                                    __LINE__))

#define PRINT(expr)\
        cout<<#expr<<endl;


int main()
{
    cout<<__PRETTY_FUNCTION__<<endl;
    cout<<__FILE__<<endl;
    cout<<__LINE__<<endl;
    std::string expr = "aaaa";
    
    SELF_ASSERT(1!=1);
    PRINT(asdasd);
}