###
 # @Author: Liu Weilong
 # @Date: 2021-07-06 20:28:40
 # @LastEditors: Liu Weilong
 # @LastEditTime: 2021-07-07 12:11:50
 # @Description: 学习 如何 使用变量
### 

#!/bin/bash

# 变量赋值
var_1="hello world"
echo $var_1

# 让变量变为只读
# readonly var_1

# 删除变量(只读变量无法取消)
unset var_1

# bash 中的for 循环

files=$(ls /etc)
echo ${files[@]}
for file in $(ls /etc)
    do echo $file
done

# 数组的定义 内部所有的都是变量吗？
values=(value0 value1)

values[0]='hello'
values[1]='world'

echo ${values[@]}

# 这个地方仅仅找到了一个变量不知道为什么
for value in $values
    do echo $value
done

# 得到数组长度
j=${#values[@]}
# 进行循环
for ((i=0;i<j;i++))
    do echo ${values[$i]}
done