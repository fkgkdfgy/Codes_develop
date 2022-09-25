<!--
 * @Author: Liu Weilong
 * @Date: 2020-12-28 10:21:44
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-12-28 11:16:56
 * @FilePath: /3rd-test-learning/doc/SLAM-related/ORB-SLAM2(Stereo)/costfunction_in_orb.md
 * @Description: 
-->
### PoseOptimization

EdgeStereoSE3ProjectXYZOnlyPose<br>
内部公式
$$ 
    d_p = p_{obs} - \cfrac{(R{p_{world}}-t)}{Z}
$$

地面约束的添加