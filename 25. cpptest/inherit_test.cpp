/*
 * @Author: Liu Weilong
 * @Date: 2021-01-21 21:54:58
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-01-21 22:07:05
 * @Description: 
 *              这个部分主要是为了测试继承和虚指针 调用的关系
 *              主要测试内容：
 *              1. 在构造函数中，基类还是会调用自己下面定义的函数 比如 base print 的Init() 
 *                 func2() 虽然在Derived里面进行了定义，但是在Base 的ctor 中会被认为是无定义
 *              2. 在动态绑定的过程中，func1() 就会调用派生类
 *              3. 关于继承模板父类的子类无法直接使用父类成员的问题: 
 *                 这个问题最后发现是和   !!模板在编译的时候进行搜索的机制有关系!!
 *                 所以使用和父类的成员的时候，尽量使用 this-> 来进行使用吧。
 *              4. 继续昨天的问题，突然那想到如果是存在一个重名成员应该怎么办？？ 
 *                 答案是会进行覆盖，除非进行显式地说明使用父类的成员。
 *                  看来不行的话，之后还是进行显式地指明吧
 *                 
 * 
 */

#include <iostream>
#include <typeinfo>

using namespace std;

class Base
{
    public:

    Base(){Init();}
    virtual void Init(){cout<<"base init "<<endl;}

    virtual void func1(){func2();}
    virtual void func2()=0;
    int content = 0;
};

class Derived:public Base
{
    public:
    Derived(){Init();}
    virtual void Init() override{cout<<"derived init"<<endl;};
    virtual void func2() override {cout<<"derived print "<< content<<endl;}
};

template <typename C, int N>
class TemplateBase
{
    public:
    TemplateBase(){cout<<"template base init"<<endl;}
    C template_class;
    const int template_num = N;
    void print_template_base()const{cout<<"template class"<<typeid(C).name()<<"template number"<<template_num<<endl;}
};

template <typename C, int N>
class TemplateDerived: public TemplateBase<C,N>
{
    public:
    TemplateDerived(){}
    
    void print_template_derived()const{
        // cout<<"template class"<<typeid(C).name()<<"template number"<<template_num<<endl;   ///  这样写编译是无法通过的
        cout<<"template class "<<typeid(C).name()<<" template number "<<this->template_num<<endl;   ///  这样写编译是 可以 通过的
        cout<<"template class "<<typeid(C).name()<<" template number "<<TemplateBase<C,N>::template_num<<endl;   ///  这样写编译是 可以 通过的

    }

    const int template_num = 444;


};



int main()
{
    TemplateDerived<double,5> td;
    td.print_template_derived();
}

