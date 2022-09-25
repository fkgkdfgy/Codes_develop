###
 # @Author: Liu Weilong
 # @Date: 2021-07-07 12:13:05
 # @LastEditors: Liu Weilong
 # @LastEditTime: 2021-07-07 15:21:21
 # @Description: bash 文件 循环具体如何编写
### 

#! /bin/bash 

# C 类型的循环

j=$1
for ((i=1; i<=j; i++))
do
touch file$i && echo file $i is ok
done
