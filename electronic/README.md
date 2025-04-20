# Esquema electrónico principal

![](<../img/electronic PCB 3D.png>)

El proyecto se desarrolló usando [KiCAD](https://www.kicad.org/). [electronic.kicad_pro](electronic.kicad_pro) se desarrollo para comunicar multiples sensores a una placa de ESP32. Los componentes utilizados para esta PCB son:

* 1 **ESP32 WROOM-32** DevKit
* 1 Módulo **MPU6050**
* 1 Módulo **BMP280**
* 1 Módulo **NEO6M GPS**
* 1 Pantalla **OLED 128x64**
* 1 Sensor **DHT22**
* 1 Resistencia de 10K $\Omega$
* 4 Módulos **HC-SR04**
* 1 Conector **B2B-XH-A** para la batería
* 1 Conector **B4B-XH-A** para los motores
* **PinSockets** de 2.54mm

## Esquema
![](<../img/electronic.svg>)

El esquema electrónico se encuentra dividido en 8 partes:

* 5 partes indican los componentes que contienen los módulos **MPU6050**, **BMP280**, **NEO6M GPS**, **OLED 128x64** y **DHT22** y etiquetas de los pines de conexión.
* 1 parte indica la conexión de los módulos **HC-SR04** y etiquetas para los pines de conexión de cada módulo.
* 1 parte indica la conexión de los pines de control de los motores y la lectura del estado de la batería.
* La última parte indica la conexión del ESP32 con los componentes mencionados anteriormente.

## PCB
![](<../img/electronic PCB.svg>)

El diseño de la PCB contiene terminales de tipo PinSocket de 2.54mm para la conexión de los módulos **BMP280**, **NEO6M GPS**, **OLED 128x64**, **HC-SR04** y el **ESP32 WROOM-32**. También contiene un conector de tipo **B2B-XH-A** para la lectura de la batería y un conector de tipo **B4B-XH-A** para el control de los motores. Por último, el diseño de la PCB incluye el módulo **MPU6050**, el sensor **DHT22** y la resistencia de 10K $\Omega$.


