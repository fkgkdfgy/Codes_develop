'''
Author: Liu Weilong
Date: 2021-02-11 18:23:23
LastEditors: Liu Weilong
LastEditTime: 2021-02-11 18:33:26
Description: 

数据类型：
1. List
2. Tuple

'''
# 1. list
list_1=[12,23.4,'sds','''sdsdsd''']

for i in list_1:
    print(i)
    print(type(i))

print("the length of list ")
len(list_1)

print("the last two obj is "+str(list_1[-2]))

# 2. Tuple
a = 1.0
tuple_1 = (list_1,a,3,[])

for i in tuple_1:
    print(i);print(type(i))    


a = 1000.0
print(tuple_1[1])
print(type(tuple_1[1]))



# 3. 