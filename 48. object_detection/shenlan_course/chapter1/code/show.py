'''
Author: Liu Weilong
Date: 2021-06-06 18:02:48
LastEditors: Liu Weilong
LastEditTime: 2021-06-06 23:48:33
Description: 
画精度召回率曲线

'''

import matplotlib.pyplot as plt

plt.figure("精度召回率曲线")
plt.xlabel("Recall")
plt.ylabel("Precision")
plt.plot([0.8,0.6,0.6,0.4,0.2],[0.6667,0.6,0.75,0.6667,1],'o-')
plt.show()
