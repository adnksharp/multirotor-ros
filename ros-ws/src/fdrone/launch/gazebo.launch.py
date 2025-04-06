from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess, SetEnvironmentVariable
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    package_name = 'fdrone'
    urdf_file_name = 'model.urdf'

    urdf_path = os.path.join(
        get_package_share_directory(package_name),
        'urdf',
        urdf_file_name
    )

    gazebo = ExecuteProcess(
        cmd=['gz', 'sim', '-r', '-v', '4', 'empty.sdf'],
        output='screen'
    )

    spawn = Node(
        package='ros_gz_sim',
        executable='create',
        arguments=[
            '-name', 'fdrone',
            '-x', '0.0',
            '-y', '0.0',
            '-z', '0.0',
            '-R', '0.0',
            '-P', '0.0',
            '-Y', '0.0',
            '-file', urdf_path
        ],
        output='screen'
    )

    publish = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{
            'robot_description': open(urdf_path).read()
        }],
        output='screen'
    )

    ros_gz_bridge_node = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        arguments=[
            '/cmd_vel@geometry_msgs/msg/Twist[ignition.msgs.Twist'
        ],
        remappings=[
            ('/cmd_vel', '/fdrone/cmd_vel')
        ],
        output='screen'
    )

    return LaunchDescription([
        gazebo,
        spawn,
        publish,
        ros_gz_bridge_node
    ])
