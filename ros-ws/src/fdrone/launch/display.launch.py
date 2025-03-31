from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare

def generate_launch_description() -> LaunchDescription:
    ld = LaunchDescription()

    # Obtener la ruta del paquete
    pkg_share = FindPackageShare(package='fdrone')

    # Construir rutas de archivos
    default_model_path = PathJoinSubstitution(['urdf', 'model.urdf'])
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


    # Agregar tu nodo joint_state_publisher personalizado
    jsp_node = Node(
        package='fdrone',
        executable='pos_pub.py',
        name='joint_state_publisher'
    )
    ld.add_action(jsp_node)

    # Incluir el lanzamiento de urdf_launch, pero sin el joint_state_publisher predeterminado y sin la GUI
    ild: IncludeLaunchDescription = IncludeLaunchDescription(
        PathJoinSubstitution([FindPackageShare('urdf_launch'), 'launch', 'display.launch.py']),
        launch_arguments={
            'urdf_package': 'fdrone',
            'urdf_package_path': LaunchConfiguration('model'),
            'rviz_config': LaunchConfiguration('rvizconfig'),
            'jsp_gui': LaunchConfiguration('gui'),  # Usar el argumento 'gui' para desactivar la GUI
            'use_jsp': 'false'
        }.items()
    )
    ld.add_action(ild)
    
    return ld
