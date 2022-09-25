<!--
 * @Author: Liu Weilong
 * @Date: 2021-01-29 14:01:39
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-04-24 16:59:49
 * @FilePath: /Codes/31. orb_slam_related/origin/doc/ORB_loopclosing.md
 * @Description: 
-->

需要进行补充


### ORB LoopClosing
内部来自 LoopClosing.h 和 LoopClosing.cc

1. DetectLoop
2. ComputeSim3
3. CorrectLoop
   
-----
Sim3 群的优化见 Supplement Material
Sim3 的几何校验见 Supplement Material

-----

1.a. DetectLoop\
a.1. 从CurrentFrame的共视帧得到最小得分
a.2. 最小得分作为阈值,进行LoopClosure帧的搜索，具体操作基本和 Tracking Relocalization 相同-> ORB_tracking.md:2.a.1
a.3. 连续性检测 代码实现的有点懵
     for_each CandidateKF
        通过共视关系，形成一个新的ConsistencyGroup(这个之后叫new Group)
        for_each mConsistencyGroup(这个之后叫old Group)
            new Group 和 Old Group 进行count，查看是否含有相同KF
            if(有)
                new Group 加入vConsistencyGroup_new
                并且连续计数+1
            if(连续计数>th)
                当前KF加入 mvpEnoughConsistentCandidates
        
        if(和old Groups没有任何关系)
            new Group 加入vConsistencyGroup_new
            连续计数置1

2.a. ComputeSim3 进行几何校验 和 scale 恢复\
a.1. 对每一个满足条件的KF
      和mCurrentKF 进行SearchByBows，如果inliners 数量达标，为其创造Sim3Solver
      并保存这个Sim3Solver 每一个Sim3Solver 都保存了 mCurrentKF* 和 candidate KF*
a.2. 对保存的每一个Sim3Solver
      进行 Sim3 计算，
      得到s,R,t 之后，SearchBySim3 进一步匹配，
      匹配之后，进行OpptimizeSim3 的优化
      如果匹配的比较好就可以跳出了,匹配的不好就继续计算下一个Sim3Solver
a.3. 统计匹配的比较好的KF 的共视关键帧
      得到这些关键帧所有的MapPoints
a.4. SearchByProjection，进一步进行匹配

3.a. CorrectLoop

            

   