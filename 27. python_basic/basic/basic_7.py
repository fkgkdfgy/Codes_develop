'''
Author: Liu Weilong
Date: 2021-02-11 18:48:19
LastEditors: Liu Weilong
LastEditTime: 2021-02-14 15:42:08
Description: 

主要内容：
1. 函数的定义
1.1 如何定义
1.2 类型assert
1.3 return
1.4 参数使用 


'''
# 一定要记住加上:啊啊啊啊
def sum1(num1,num2):
    return num1+num2

print(sum1(2,3.0))
print(sum1([232,32,'sd'],['sdsdes']))

# sum2 版本加上 isinstance 类型检查
def sum2(num1,num2):
    if not (isinstance(num1,(int,float)) and isinstance(num2,(int,float))):
        # raise 是什么意思暂时还不清楚
        raise TypeError('参数类型存在错误')
    return sum1(num1,num2)
print(sum2(2,3.0))
# print(sum2(2,'11'))

# division 做一个整数除函数
# return 如果不进行特意的声明 会返回Union

def division1(num1,num2):
    # 查看一下 会不会有自动转换
    if not (isinstance(num1,int) and isinstance(num2,int)):
        raise TypeError('参数类型错误')
    else:
        m = num1 % num2
        n = (num1-m)/num2
        return n,m;

def division2(num1,num2):
    # 查看一下 会不会有自动转换
    if not isinstance(num1,int) or not isinstance(num2,int):
        raise TypeError('参数类型错误')
    else:
        m = num1 % num2
        n = (num1-m)/num2
        return [n,m];

print(division1(12,7))
print(division2(12,7))

# 1.4 参数使用：默认参数、关键字参数、不定长参数

# 默认参数 和 C++ 一致，必须定义在右边
def print_user_info1(name,age,sex="男"):
    # 原来这里的打印可以直接逗号进行
    print(name,age,sex)

print_user_info1('aaa',"18")

# 关键字参数 , 传参顺序上的调整
print_user_info1(sex="女",age="16",name="xxxx")

# 不定长参数 hobby 就是用C++ 里面的package
def print_user_info2(name,age,sex="男",*hobby):
    # 原来这里的打印可以直接逗号进行
    print(type(hobby))
    print(name,age,sex,hobby)

print_user_info2('aaa','18','男','sdsd','sdsds','qews')

# 不定长参数 使用关键字参数 但是类型会从 tuple 变为 dict
def print_user_info3(name,age,sex="男",**hobby):
    # 原来这里的打印可以直接逗号进行
    print(type(hobby))
    print(name,age,sex,hobby)

print_user_info3( name = '两点水' , age = 18 , sex = '女', hobby = ('打篮球','打羽毛球','跑步'))


# 1.5 参数传递  引用传递 和 值传递
# 只有list 和 dict 是值传递
def unchange(b):
    b=1000.0

def change(b=[123,12]):
    b[-1] = 20000

b = 100
unchange(b)
print(b)
a=[222]
change(a)
print(a)

# 1.6 匿名函数 lamda 

sum1 = lambda a,b,c:print(a) return a+b+c;

sum1(1,2,3)