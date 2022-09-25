'''
Author: Liu Weilong
Date: 2021-06-10 17:34:44
LastEditors: Liu Weilong
LastEditTime: 2021-06-11 16:16:09
Description: 

用于分析EM 算法对于 Gaussian + Uniform 分布的适应性
'''
import numpy as np
import matplotlib.pyplot as plt

width_min = 0.0
width_max = 0.5
height_min = 0.0
height_max = 0.5

def SampleGaussianPoint(num,std_cov,mu):
    np.random.seed(0)
    x = np.random.randn(num,2)
    z = np.dot(x,std_cov)+mu

    # plt.scatter(x[:,0],x[:,1])
    # plt.show()
    # plt.axis([0,width_max,0,height_max])
    # plt.scatter(z[:,0].tolist(),z[:,1].tolist())
    # plt.show()
    return z

def SampleUniformPoint(num):
    np.random.seed(0)
    x = np.random.rand(num,2);
    z = x/2 ;
    return z

def BiFilter(max,min,data):
    a_max = np.where(data>max)
    a_min = np.where(data<min)
    rm = a_max[0].tolist()+a_min[0].tolist()
    rm =list(set(rm))
    data = np.delete(data,rm,axis=0)
    return data

# Gaussian + Uniform EM算法
def EM(data):
    




    return data


std_cov = np.mat([[0.02,0],[0,0.5]]);
mu = [0.25,0.25]
print(std_cov)
print(mu)
z_g = SampleGaussianPoint(1000,std_cov,mu)
z_u = SampleUniformPoint(3000)

z_g = BiFilter(0.5,0,z_g)
z_u = BiFilter(0.5,0,z_u)

ratio = 0.5
total = 200

num_g = int(200*ratio)
num_s = int(200*(1.-ratio))

if num_g>len(z_g):
    num_g = len(z_g)
if num_s>len(z_u):
    num_s = len(z_u)


data = np.row_stack((z_g[:int(200*0.8)],z_u[:int(200*(1-ratio))]))

np.random.shuffle(data)

plt.axis([0,width_max,0,height_max])
plt.scatter(data[:,0].tolist(),data[:,1].tolist())
plt.show()

# EM 算法主体