# Esquema electrónico secundario

![](<../img/power PCB 3D.png>)

El proyecto  [power.kicad_pro](power.kicad_pro) se desarrolló para conectar controladores ESC con motores brushless a un microcontrolador ESP32. Los componentes utilizados para esta PCB son:

* 2 resistencias de 10K $\Omega$.
* 2 resistencias de 4.7K $\Omega$.
* 1 conector de tipo **B2B-XH-A**.
* 2 conectores de tipo **B3B-XH-A**.
* 1 conector de tipo **B5B-XH-A**.
* 4 conectores de tipo **PinHeader 1x03** de 2.54mm.
* 5 teriminales para soldar cable 1x02 de 2.4mm.

## Esquema
![](<../img/power.svg>)

El esquema electrónico se encuentra dividido en 11 partes:

* 4 partes corresponden a los controladores ESC y sus etiquetas para conectar los motores brushless, la señal de control y la alimentación.
* 4 partes corresponden a los motorres brushless y sus etiquetas para conectar los controladores ESC.
* 1 parte corresponde a la conexión de una batería de 2S a los controladores ESC.
* 1 parte corresponde a la conexión del cargador de batería.
* La última parte corresponde a la conexión del microcontrolador ESP32.

## PCB
![](<../img/power PCB.svg>)

Se diseño la PCB para conectar la alimentación de los controladores ESC con la batería de 2S usando las terminales de soldadura. Los conectores PinHeader se utilizan para conectar las señales de control de los ESC y el conector B5B-XH-A se utiliza para conectar el microcontrolador ESP32 a estos pines. El conector B2B-XH-A se utiliza para leer el estado de la batería mediante divisores de voltaje hechos con resistencias de 10K $\Omega$ y 4.7K $\Omega$. El conector B3B-XH-A se utiliza para conectar el cargador de batería a la PCB.

