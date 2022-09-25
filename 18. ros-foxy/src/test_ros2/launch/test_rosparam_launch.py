import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import (DeclareLaunchArgument, GroupAction,
                            IncludeLaunchDescription, SetEnvironmentVariable)
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import (PushRosNamespace,Node)
from nav2_common.launch import RewrittenYaml

# 暂时搞不定
# 暂不进行调试
def generate_launch_description():
    
    param_dir = os.path.join(get_package_share_directory('test_ros2'),'param','test_rosparam.yaml');


    print(param_dir)
    param_dir_ = LaunchConfiguration("param_dir_",default=param_dir)
    tmp_dict={};

    # 这里的param_dir_ 必须是LaunchConfiguration 
    # 不需要进行修改的话就不需要这一步
    configure_param = RewrittenYaml(
        source_file = param_dir_,
        param_rewrites = {},
        convert_types = True
    )

    print()

    # parameter 可以直接赋值一个yaml文件的path
    return LaunchDescription([
        Node(
        package='test_ros2',
        executable='test_rosparam_node',
        name='test_rosparam_node',
        parameters=[param_dir]
        )
    ])

