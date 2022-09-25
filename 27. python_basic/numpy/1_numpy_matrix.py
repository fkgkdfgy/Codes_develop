'''
Author: Liu Weilong
Date: 2021-05-30 19:31:55
LastEditors: Liu Weilong
LastEditTime: 2021-06-14 10:39:27
Description: 
创建数组




'''
import numpy as np

# numpy的数组类型是ndarray
a = np.arange(15).reshape(3,5)

# 数组创建
b = np.array([3,2.])
print(type(b))

b = np.zeros((3,4),dtype=np.int16)
print(b)

c = np.linspace(2,10,9)
d = np.arange(0,3,0.2)

print(c)
print(d)

e = np.asmatrix(c)
print(e)
e.transpose()

a.inverse()
