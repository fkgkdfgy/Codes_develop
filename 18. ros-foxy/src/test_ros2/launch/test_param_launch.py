import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import (DeclareLaunchArgument, GroupAction,
                            IncludeLaunchDescription, SetEnvironmentVariable)
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import (PushRosNamespace,Node)


# 测试 param 
# 这个launch文件和for_launch 进行组合测试 将测试 <param> 和<include>

def generate_launch_description():

    test_string = LaunchConfiguration('test_string',
    default='TEST TEST TEST 11'
    )
    launch_dir = get_package_share_directory('test_ros2')
    

    return LaunchDescription([
        IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(launch_dir,'launch','for_include.py'))
         ,launch_arguments={'test_string':test_string}.items())
])
