'''
Author: Liu Weilong
Date: 2020-12-25 10:27:13
LastEditors: Liu Weilong
LastEditTime: 2021-02-11 09:57:13
FilePath: /3rd-test-learning/30. python_basic/basic.py
Description: 

    主要内容：
    1. 基础输入输出


'''

print('hello world')

# 三种表示
word = '嘿嘿嘿'
sequence = "这是一个句子。"
paragraph = """ 这是一个跨行，
句子。"""

# r 把string 进行反转
paragraph_raw = r"这是一个句子\n 跨行的句子"

print(word)
print(sequence)
print(paragraph)
print(paragraph_raw)

# string 的索引
print(sequence[0:-1])
print(sequence[0:3:1])

# 输入 输出 io
input_ = input("\n\n 按下 enter 之后 退出")
print("用户输入的内容是：" + input_)

# 统一行使用多个语句 只需要使用 ; 进行分割就可以了
import sys; x = '都是啊啊啊';sys.stdout.write(x+'\n')

# import & from ... import
import sys
x = "sdsds"
# for if cause 需要使用:进行结束
for i in sys.argv:
    print(i)
print("the sys path : ",sys.path)
