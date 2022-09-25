


#! /bin/bash
#  第一部分 if/else/for
#  if 有多种写法 这里我们只学习第二种

#  第一步 -e 判断 文件是否存在，如果存在就打印  source.txt exit
#  坑1: [ a ] []和a之间必须存在空格
if [ -e ./source.txt ];then
    echo "source.txt exit"
fi 

#  第二步 -s 判断长度是否大于1？？？？ 文件长度是什么意思
if [ -s ./source.txt ]; then
    echo "source.txt is longer than 0"
fi

#  第三步 -eq 判断是否相同

if [ "1000" -eq "1000" ]; then
    echo "1000 = 1000"
fi

#  第二部分 for

word1="aaa"
word2="bbb"
word3="ccc"

for i in ${word1} ${word2} $word3; do
  echo ${i}
done