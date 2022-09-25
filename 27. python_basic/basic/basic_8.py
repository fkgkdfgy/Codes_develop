'''
Author: Liu Weilong
Date: 2021-02-12 08:23:57
LastEditors: Liu Weilong
LastEditTime: 2021-02-14 15:58:52
Description: 

主要内容:
1. 迭代访问
2. 迭代器  主要是 iter () 和 next () 的使用
3. list 生成式
4. 生成器

'''
# 1. 迭代
# for 迭代 list 和 tuple
ltemp = [20,20.0,'sdsd']
print(type(ltemp))
for i in ltemp:
    print(i)

ttemp = (20,20.0,'asdasd')
print(type(ttemp))
for i in ttemp:
    print(i)

# for 迭代 string 

for i in 'asdas':
    print(i)

# 2. 迭代器
# next 返回指向，并且 + 1

len_l = len(ltemp)
len_t = len(ttemp)

iter_l = iter(ltemp)
iter_t = iter(ttemp)

for i in range(0,len_l):
    print(next(iter_l),end=" ")

print()

# 3. list 生成式
list1= [(x+1,y+1) for x in range(3) for y in range(5)] 
print(list1)
list2= [x * x for x in range(1, 11) if x % 2 == 0]
print(list2)

# 4. 生成器
# []() 注意这里的不同
gen=(x*x for x in range(10))
print(gen)

for num in gen:
    print(num)

# yield 的使用, 和return 的不同就是下次  next的使用 会从上一次 yield 的位置继续向下走，直到遇到下一个yield
def odd():
    print("1step")
    yield 1
    print("2step")
    yield 2
    print("3step")
    yield 3

o = odd()
print(next(o))
print(next(o))
print(next(o))

# 打印杨辉三角

def triangles():
    L = [1]
    while True:
        yield L
        L.append(0)
        L=[L[n-1]+L[n] for n in range(len(L))]

t = triangles()
for i in range(10):
    print(next(t))
