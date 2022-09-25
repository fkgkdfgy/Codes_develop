'''
Author: Liu Weilong
Date: 2021-04-22 15:58:15
LastEditors: Liu Weilong 
LastEditTime: 2021-04-22 16:00:17
FilePath: /Codes/27. python_basic/python_practice_msckf/config.py
Description: 
'''
import numpy as np 
import cv2

class OptimizationCnfigEuRoc(object):
    """
    Configuration parameters for 3d feature position optimization
    """
    def __init__(self):
        self.translation_threshold = -1.0  # 0.2
        self.huber_epsilon = 0.01
        self.estimation_precision = 5e-7
        self.initial_damping = 1e-3
        self.outer_loop_max_iteration = 5 # 10
        self.inner_loop_max_iteration = 5 # 10

class ConfigEuRoc(object):