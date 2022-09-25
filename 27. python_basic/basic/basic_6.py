'''
Author: Liu Weilong
Date: 2021-02-11 18:36:59
LastEditors: Liu Weilong
LastEditTime: 2021-02-11 18:47:39
Description: 

主要内容：
1. 条件语句
2. 循环

'''
# 1. 条件语句

result = 81

if (result>90) or (result<-90):
    print("A+")
elif (result>80) or (result<-80):
    print("A")
else:
    print("B")

# 2. 循环语句

ltemp = [10.0,2.0,3.0,4.0]
for i in ltemp:
    print(i)
for i in range(2,4):
    print(ltemp[i])

# range 的 使用
a =range(1,9,3)
print(type(list(a)))

print(range(1,9,3))

print(type(range(0,8,2)))

# while 

while len(ltemp)!=0:
    obj = ltemp.pop();print(obj)