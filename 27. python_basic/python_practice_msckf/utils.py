'''
Author: Liu Weilong
Date: 2021-04-22 16:07:38
LastEditors: Liu Weilong 
LastEditTime: 2021-04-22 16:10:49
FilePath: /Codes/27. python_basic/python_practice_msckf/utils.py
Description: 
'''
'''

主要是为了进行计算方面的学习

'''

import numpy as np

def skew(vec):
    '''
    Create a skew-symmetric matrix form a vector3d
    '''
    x,y,z = vec
    return np.array([
        [0,-z,y],
        [z,0,-x],
        [-y,x,0]
    ])

if __name__ == '__main__':
    vec = [0,2,3];
    print(skew(vec))