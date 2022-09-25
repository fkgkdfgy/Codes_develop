#! /bin/bash 
#  读取 输入 然后输出
read -p "please input your info ": input_info

# ${#a} 获得字符串a 的长度
if [ ! ${#input_info} -eq 0 ]; then
    echo $input_info
    echo "no_input3"
fi

echo "no_input1"

if [ -z $input_info ];then 
    echo "no_input2"
fi


