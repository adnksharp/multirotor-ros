from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, ExecuteProcess
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare

def generate_launch_description() -> LaunchDescription:
    ld = LaunchDescription()

    # Obtener la ruta del paquete
    pkg_share = FindPackageShare(package='fdrone')

    # Construir rutas de archivos
    default_model_path = PathJoinSubstitution([pkg_share, 'urdf', 'model.urdf'])
    default_rviz_path = PathJoinSubstitution([pkg_share, 'rviz', 'urdf.rviz'])

    # Argumento para habilitar/deshabilitar GUI
    gui_arg = DeclareLaunchArgument(
        name='gui',
        default_value='false',  # Establecer a false para eliminar la GUI
        choices=['false', 'true'],
        description='Flag to enable/disable the GUI'
    )
    ld.add_action(gui_arg)

    # Argumento para el archivo de configuración de RViz
    rviz_arg = DeclareLaunchArgument(
        name='rvizconfig',
        default_value=default_rviz_path,
        description='Path to the RViz config file relative to the package'
    )
    ld.add_action(rviz_arg)

    # Argumento para el modelo URDF
    model_arg: DeclareLaunchArgument = DeclareLaunchArgument(
        name='model',
        default_value=default_model_path,
        description='Path to robot urdf file relative to urdf_tutorial package')
    ld.add_action(model_arg)


    jsp_node = Node(
        package='fdrone',
        executable='pos_get.py',
        name='robot_position_publisher',
    )
    ld.add_action(jsp_node)

    # Incluir el lanzamiento de urdf_launch, pero sin el joint_state_publisher predeterminado y sin la GUI
    ild: IncludeLaunchDescription = IncludeLaunchDescription(
        PathJoinSubstitution([FindPackageShare('urdf_launch'), 'launch', 'display.launch.py']),
        launch_arguments={
            'urdf_package': 'fdrone',
            'urdf_package_path': LaunchConfiguration('model'),
            'rviz_config': LaunchConfiguration('rvizconfig'),
            'jsp_gui': LaunchConfiguration('gui'),
            'use_jsp': 'false'
        }.items()
    )
    ld.add_action(ild)

    # gz create
    gz_node = Node(
        package='ros_gz_sim',
        executable='create',
        name='spawner',
        output='screen',
        arguments=[
            '-name', 'fdrone',
            '-topic', '/robot_description',
        ]
    )
    ld.add_action(gz_node)

    # gz bridge
    gz_bridge_node = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        name='robot_gz_bridge',
        output='screen',
        parameters=[{'use_sim_time': True}],
        arguments=[
            '/world/empty/model/fdrone/joint/rotor_0_joint/cmd_vel@std_msgs/msg/Float64]gz.msgs.Double',
            '/world/empty/model/fdrone/joint/rotor_0_joint/state@sensor_msgs/msg/JointState[gz.msgs.Model',
            '/world/empty/model/fdrone/joint/rotor_1_joint/cmd_vel@std_msgs/msg/Float64]gz.msgs.Double',
            '/world/empty/model/fdrone/joint/rotor_1_joint/state@sensor_msgs/msg/JointState[gz.msgs.Model',
            '/world/empty/model/fdrone/joint/rotor_2_joint/cmd_vel@std_msgs/msg/Float64]gz.msgs.Double',
            '/world/empty/model/fdrone/joint/rotor_2_joint/state@sensor_msgs/msg/JointState[gz.msgs.Model',
            '/world/empty/model/fdrone/joint/rotor_3_joint/cmd_vel@std_msgs/msg/Float64]gz.msgs.Double',
            '/world/empty/model/fdrone/joint/rotor_3_joint/state@sensor_msgs/msg/JointState[gz.msgs.Model',
        ]
    )
    ld.add_action(gz_bridge_node)
 
    # gz sim
    gz_sim_node = ExecuteProcess(
        cmd=['gz', 'sim', '-r', '-v', '4', 'empty.sdf'],
        output='screen',
    )
    ld.add_action(gz_sim_node)
    return ld
