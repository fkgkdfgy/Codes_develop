import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import (DeclareLaunchArgument, GroupAction,
                            IncludeLaunchDescription, SetEnvironmentVariable)
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import (PushRosNamespace,Node)



def generate_launch_description():
    # 目前看来不能给node remap一个名字
    # 这里要要注意remap 的顺序，('1','2') 1 是<node>.cpp 内部定义好的名字 2 是1会被remap 在外部进行使用的名字
    remapping_1 = [('test_message_','remap_test_message_')]
    remapping_2 = [('aaaa','remap_test_message_')]
    return LaunchDescription([
        Node(
        package='test_ros2',
        executable= 'test_remap_pub_node',
        name ='test_remap_pub_node',
        remappings= remapping_1
        ),
        Node(
        package='test_ros2',
        executable= 'test_remap_sub_node',
        name ='test_remap_sub_node',
        remappings= remapping_2
        )
        # 需要给发布方添加remap
    ])