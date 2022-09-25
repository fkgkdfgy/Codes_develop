/*
 * @Author: Liu Weilong
 * @Date: 2020-12-08 16:12:13
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-12-14 14:50:56
 * @FilePath: /3rd-test-learning/11. utils/dataset_fitting/dataset_fitting_test.cpp
 * @Description: 
 */
#include "save_result.h"

int main()
{
    EuRocToTUMORBInterf("/home/sunny/workspace_public/Dataset/EuRoC/MH02/mav0/state_groundtruth_estimate0/data.csv",
                        );
    // string data_line = "1403636859536666368,4.621150,-1.837605,0.739627,0.567395,-0.129040,-0.810903,-0.062030,-0.023437,0.061278,0.432133,-0.002535,0.021162,0.077170,-0.024346,0.144439,0.067540";
    // EuRocTimeStamp(data_line);
    // EuRocTimeStampORB(data_line);
}