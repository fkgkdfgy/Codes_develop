<!--
 * @Author: Liu Weilong
 * @Date: 2021-01-26 14:04:09
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-26 14:27:42
 * @FilePath: /3rd-test-learning/ground_constraint.md
 * @Description: 
-->
### EdgeGroundConstraintZ 
$$
    r = e_3^Tt_{bc} - e_3^{T}(-R_{cw}^{T}t_{cw}) 
    \\
    e_3 = [0,0,1]^{T}
$$

### EdgeGroundConstraintZRP
$$
    r_1 = e_3^Tt_{bc} - e_3^{T}(-R_{cw}^{T}t_{cw}) 
    \\
    r_2 = \left[
        \begin{aligned}
            e_1^T\\e_2^T
        \end{aligned}\right]In([R_{bc}R_{cw}]^{T})
$$