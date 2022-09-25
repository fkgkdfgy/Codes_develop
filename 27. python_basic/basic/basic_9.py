'''
Author: Liu Weilong
Date: 2021-02-14 16:00:58
LastEditors: Liu Weilong
LastEditTime: 2021-02-14 17:40:29
Description: 

主要内容：面向对象的代码
1. 类的定义
2. 类的构造和析构
3. 类的继承


'''
# 注意事项：
# 1. 这里多了一个() 需要注意
# 2. @classmethod 声明是类方法?? 这里的类方法是个什么鬼奥
#    果然 classmethod 中的 var 和 实例化之后的var 不是一套内容
# 3. type1的一个cls代表一个自指针，cls 并不是一个关键字，可以随意更换
class Basic():
    # 类变量
    var1 = 10
    var2 = 10.0
    var3 = ['sdas'] 

    def __init__(self):
        self._age = 100.0
        # _age 实例成员变量

    @classmethod
    def type1(cls):
        print(type(cls.var1))
        print(cls.var1)
    @classmethod
    def type2(cls):
        print(type(cls.var2))
    
    # classmethod 感觉像是 static？的类别
    @classmethod
    def type3(cls):
        print(type(cls.var3))   
    # 实例化才能使用的函数
    def type4(self):
        print('asdads',self.var1)

a = Basic()
b = Basic()

a.var1 = 1000.0
b.var1 = 2000.0

a.type1()
b.type1()
Basic.type1()

a.type4()
b.type4()
# Basic.type4() 会报错 因为这是一个用于实例化对象的函数


# 类变量、类方法修改
Basic.var1 = 200.0
Basic.type1()


def changeType(self):
    self.var1 = 999
    print(type(self.var1))
    print(self.var1)

Basic.type4 = changeType

a.type4()

class Basic2():
    var1 = 1.0
    var2 = 1.0
    str1 = 1.0
    
    def __init__(self,str_t):
        str1 = str_t
        print('ctor called')
    
    def __del__(self):
        print('dtor called')

d = Basic2('sdsd')
del d


class Basic3():
    def __init__(self,account,age):
        self._age = age
        self.__account = account   # 两个下划线私有类型，无法直接访问
    
    def __del__(self):
        print('dtor called')


# !继承的内容存在问题，之后进行探索
class Derived(Basic3):
    pass

e = Basic3(2323,4242)

# 自动的补全，已经找不到__account 这个变量了，但是可以用过下面的方式进行访问
print(e._Basic3__account)