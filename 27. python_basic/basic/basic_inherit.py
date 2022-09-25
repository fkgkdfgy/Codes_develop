#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
'''
Author: Liu Weilong
Date: 2021-02-14 19:06:36
LastEditors: Liu Weilong
LastEditTime: 2021-02-14 19:49:22
Description: 

主要是检测一下python的继承内容

试一下 继承  pass 和 super

注意 python3 使用super 进行搜索的时候，使用的是广度有限的策略

'''


import random as r

class Fish:
    def __init__(self,name, number):
        self._name = name
        self._number = number
        self._coordinate = [r.randint(0,10),r.randint(0,100)]
        print('ctor ')

    def move(self):
        for i in range(len(self._coordinate)):
            self._coordinate[i] = self._coordinate[i] + r.randint(0,10)    
            
    def name(self):
        print(self._name)
        return self._name
    
    def coordinate(self):
        print(self._coordinate)
        return self._coordinate
    
    def number(self):
        print('in Fish',self._number)
        return self._number

fish = Fish("small",123.0);

fish.number()
fish.coordinate()
fish.move()
fish.coordinate()
fish.name()

# 
class GoldenFish(Fish):
    
    def number(self):
        print("in GoldenFish",self._number)
        return self._number

    def print_number(self):
        print("in print number")
        super().name()

    pass

fish = GoldenFish("gf",1232);
fish.number()
fish.coordinate()
fish.move()
fish.coordinate()
fish.name()
fish.print_number()


class Shark(Fish):
    def __init__(self,name,number,size):
        self._size = size
        super().__init__(name,number) # 如果注释掉这一行 Fish 内部的 _name _number 没有进行实例化， 后续函数会报错

    def size(self):
        print(self._size)
        return self._size
    
fish = Shark('big',444,'big')
fish.number()
fish.coordinate()
fish.move()
fish.coordinate()
fish.name()
fish.size()
