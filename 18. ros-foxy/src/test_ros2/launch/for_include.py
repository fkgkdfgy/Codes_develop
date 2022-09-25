
from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import (DeclareLaunchArgument, GroupAction,
                            IncludeLaunchDescription, SetEnvironmentVariable)
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import (PushRosNamespace,Node)


# 测试 param

def generate_launch_description():

    test_string = LaunchConfiguration('test_string',
    default='TEST TEST TEST'
    )

    return LaunchDescription([
        Node(
            package='test_ros2',
            executable='test_param_node',
            name='www',
            parameters=[{'test_string':test_string}],
            arguments=["xxx"]
        )
    ])
