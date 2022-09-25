# launch 文件编写(Simple 版)

### ROS1 ROS2 语句对照

1.Node

        Node(
        package= 'test_ros2',
        executable ='test_param_node',
        name = 'test_param_node',
        output = 'screen',
        parameters= [{'test_string':test_string}],
        arguments= [" xxxx"]
        )

对应ros1 launch 文件中的\<node>
Parameters 从ROS 系统对.cpp 文件传参 对应了 node.declare_parameter 
Arguemnts 直接从argv 对.cpp 传参

2.Arg

       test_string = LaunchConfiguration('test_string',
       default='TEST TEST TEST 11'
       )

直接对应ros1 launch 文件中的\<arg>

3.Include

        LaunchDescription([
        IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(launch_dir,'launch',  
        'for_include.py'))
         ,launch_arguments={'test_string':test_string}.items())

直接对应ros1 launch 文件中的\<include>

4.Remapping

        remapping = [('test_message_','remap_test_message_')]

        Node(
        package='test_ros2',
        executable= 'test_remap_pub_node',
        name ='test_remap_pub_node',
        remappings= remapping
        )
直接对应ros1 launch 文件中的\<remap>

