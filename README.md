# Multirotor-ROS

![](https://i.imgur.com/7XxNan8.png)

Plataforma de adquisición de datos de un multirotor con ESP32 y ROS2

## Requisitos

<details>
  <summary>Paquetes</summary>

  ```shell
  sudo apt install gedit curl git python3-pip clang software-properties-common wget power-profiles-daemon
  ```
</details>

<details>
  <summary>ROS2</summary>

  ```shell
  sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
  echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null
  sudo apt update ; sudo apt upgrade
  sudo apt install -y \
  python3-flake8-blind-except \
  python3-flake8-class-newline \
  python3-flake8-deprecated \
  python3-mypy \
  python3-pip \
  python3-pytest \
  python3-pytest-cov \
  python3-pytest-mock \
  python3-pytest-repeat \
  python3-pytest-rerunfailures \
  python3-pytest-runner \
  python3-pytest-timeout \
  python3-colcon-common-extensions \
  ros-dev-tools \
  ```

  #### Ubuntu 24
  ```shell
  sudo apt install ros-jazzy-desktop
  ```

  #### Ubuntu 22
  ```shell
  sudo apt install ros-humble-desktop
  ```

  #### Ubuntu 20
  ```shell
  sudo apt install ros-foxy-desktop
  ```
</details>

<details>
  <summary>Shortcuts</summary>

  ### Bash

  ```shell
  gedit ~/.bashrc
  ```

  Agregar las siguientes líneas al final del archivo
  ```shell
  run-ros() 
  {
     export ROS_DOMAIN_ID=21
     export ROS_VERSION=2
     export ROS_PYTHON_VERSION=3
     export ROS_DISTRO=jazzy
     source /opt/ros/jazzy/setup.bash
  }

  run-uros() 
  {
	 export ROS_DOMAIN_ID=21
	 export ROS_VERSION=2
	 export ROS_PYTHON_VERSION=3
	 export ROS_DISTRO=jazzy
	 source /opt/ros/jazzy/setup.bash
	 source ~/uros-ws/install/local_setup.bash
  }
  ```

  > Reemplazar `jazzy` por la versión de ROS2 que se tenga instalada

  ### Zsh

  ```shell
  gedit ~/.zshrc
  ```

  Agregar las siguientes líneas al final del archivo
  ```shell
  run-ros() 
  {
     export ROS_DOMAIN_ID=49
     export ROS_VERSION=2
     export ROS_PYTHON_VERSION=3
     export ROS_DISTRO=jazzy
     source /opt/ros/jazzy/setup.zsh
  }

  run-uros() 
  {
     export ROS_DOMAIN_ID=49
	 export ROS_VERSION=2
	 export ROS_PYTHON_VERSION=3
	 export ROS_DISTRO=jazzy
	 source /opt/ros/jazzy/setup.zsh
	 source ~/uros-ws/install/local_setup.zsh
  }
  ```
</details>

<details>
  <summary>gazebo</summary>

   #### Ubuntu 24
  ```shell
  sudo apt-get install ros-${ROS_DISTRO}-ros-gz
  ```

</details>

<details>
  <summary>uROS</summary>

  ```shell
  run-ros
  mkdir ~/uros-ws
  cd ~/uros-ws
  git clone -b $ROS_DISTRO https://github.com/micro-ROS/micro_ros_setup.git src/micro_ros_setup
  sudo apt update && rosdep update
  rosdep install --from-paths src --ignore-src -y
  colcon build
  ```

  ### Bash

  ```shell
  source ~/uros-ws/install/local_setup.bash
  ros2 run micro_ros_setup create_agent_ws.sh
  ros2 run micro_ros_setup build_agent.sh
  source install/local_setup.bash
  ```

  ### Zsh

  ```shell
  source ~/uros-ws/install/local_setup.zsh
  ros2 run micro_ros_setup create_agent_ws.sh
  ros2 run micro_ros_setup build_agent.sh
  source install/local_setup.bash
  ```

</details>

<details>
  <summary>Arduino</summary>

  ### Arduino IDE
  ```shell
  sudo add-apt-repository ppa:flatpak/stable
  sudo apt update ; sudo apt upgrade
  sudo apt install flatpak gnome-software-plugin-flatpak
  flatpak remote-add --if-not-exists flathub https://dl.flathub.org/repo/flathub.flatpakrepo
  flatpak install flathub cc.arduino.IDE2
  sudo usermod -a -G dialout $USER
  ```

  Instalar `esp32 by Espressif` desde el gestor de tarjetas de Arduino
  ![](https://i.imgur.com/3CAvWoM.png)

  > [!NOTE]
  > Agregar la librería `Adafruit_Sensor` desde el gestor de librerías de Arduino

</details>

<details>
  <summary>Librería `uROS` para Arduino</summary>

  ```shell
  run-ros
  git clone -b $ROS_DISTRO https://github.com/micro-ROS/micro_ros_arduino.git ~/Arduino/libraries/micro_ros_arduino
  ```
</details>

## Software y Hardware Utilizado

* ESP32 WROOM 32 DevKit
* MPU6050
* [ROS2 Jazzy](https://aur.archlinux.org/packages/ros2-jazzy-base)
* [uROS Jazzy](https://github.com/micro-ROS/micro_ros_setup/tree/jazzy)
* [uROS Arduino Jazzy](https://github.com/micro-ROS/micro_ros_arduino/tree/jazzy)

## Clonar Repositorio

```shell
mkdir ~/Proyectos
cd ~/Proyectos
git clone https://github.com/adnksharp/multirotor-ros.git
```

## Construir el workspace de ROS2

```shell
run-ros
cd ~/Proyectos/multirotor-ros/ros-ws
colcon build
```

<details>
  <summary>Bash</summary>

```shell
source install/setup.bash
 ```

</details>

<details>
  <summary>Zsh</summary>

```shell
source install/setup.zsh
 ```

</details>

## Lanzar gazebo

```shell
ros2 launch fdrone gazebo.launch.py
```

<!--
## Compilar sketch de Arduino

`Ctrl + O` para abrir el archivo `~/Proyectos/multirotor-ros/ino/talk/talk.ino` desde la IDE de Arduino

> [!WARNING]
> Actualmente solo las versiones `iron` y `humble` de `uROS` están actualizadas a la versión `2.0.7`, es posible que la IDE de Arduino recomiende actualizar la librería `uROS` a esa versión.
> Ignorar la actualización y compilar el sketch.

### Configuración de la tarjeta
Crear un archivo en la carpeta `~/Proyectos/multirotor-ros/ino/talk/` con el nombre
`wificfg.h` y agregar las siguientes líneas

```cpp
char* ssid = "NOMBRE_DE_LA_RED_WIFI";
char* pass = "CONTRASEÑA_DE_LA_RED_WIFI";
char* host = "IP_DE_LA_PC_DONDE_SE_EJECUTA_ROS2";
int port = 8888;
```

### Compilar y subir sketch

![](https://i.imgur.com/K5Q0o10.png)

## Ejecutar el agente de uROS

```shell
run-uros
run micro_ros_agent micro_ros_agent udp4 --port 8888
```

![](img/out1.svg)

## Ejecutar el nodo de escucha de ROS2

### Bash
```shell
run-ros
source ~/Proyectos/multirotor-ros/ros-ws/install/setup.bash
ros2 run main listener
```

### Zsh
```shell
run-ros
source ~/Proyectos/multirotor-ros/ros-ws/install/setup.zsh
ros2 run main listener
```

![](img/out2.svg)
-->
