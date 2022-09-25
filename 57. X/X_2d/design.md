### NDT 革新

1. 相关文章的查阅
2. loam 前段进行 点线特征分离
3. ndt  分层特征进行匹配
4. fim 进行质量管理
5. 每个格子以 intensity 为观测。进行一维的em算法降噪，进一步优化内点率
6. ndt 线特征 进行hough 变化 进行反光柱匹配
7. ndt 内部加载imu先验和反光柱混合优化
8. eigenfactor 整体优化
9. loam localmap 维护
10. 单个ndt格子 成熟后不再进行更新
11. 多次累加保证分布的正确性