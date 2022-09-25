
# GMM-EM 算法实现

import numpy as np
import matplotlib.pyplot as plt

# 学习一下 python def 返回多个值
def SamplePoints():
    num1,mu1,var1 = 400,[0.5,0.5],[1,3]
    X1 = np.random.multivariate_normal(mu1,np.diag(var1),num1)
    num2,mu2,var2 = 600, [5.5, 2.5], [2, 2]
    X2 = np.random.multivariate_normal(mu2,np.diag(var2),num2)
    num3, mu3, var3 = 1000, [1, 7], [6, 2]
    X3 = np.random.multivariate_normal(mu3, np.diag(var3), num3)
    X =np.vstack((X1,X2,X3))
    return (X,X1,X2,X3)

pts,pts1,pts2,pts3 = SamplePoints()

print(pts)

plt.figure('sample points')
plt.scatter(pts1[:,0],pts1[:,1],s=5)
plt.scatter(pts2[:,0],pts2[:,1],s=5)
plt.scatter(pts3[:,0],pts3[:,1],s=5)
plt.show()

# 正式开始

# 初始化
n_cluster =3
n_points = len(pts)
Mu = [[0, -1], [6, 0], [0, 9]]
Var = [[1, 1], [1, 1], [1, 1]]
W = np.ones((n_points, n_cluster)) / n_cluster
print(W)

Pi = W.sum(axis=0) / W.sum()

print(Pi)

# E 的步骤
# 这里主要是为了更新各个点是哪个分布的概率
def updateW(X,Mu,Var,Pi):

