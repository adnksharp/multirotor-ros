// uROS libraries
#include <micro_ros_arduino.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int32.h>
#include <std_msgs/msg/int32_multi_array.h>
#include <std_msgs/msg/float32.h>
#include <std_msgs/msg/float32_multi_array.h>

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){err(LED);}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

// adafruit MPU6050 libraries
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


// hardware definitions
#define LED 2
#define DOMAIN_ID 49

// uROS configurations
char *NODE_NAME = "tester";
//char *PUB_TOPIC = "driver/talker";
//char *SUB_TOPIC = "driver/listener";
char *MPU_ACCEL_TOPIC = "driver/mpu/accel";
char *MPU_GYRO_TOPIC = "driver/mpu/gyro";
char *MPU_TEMP_TOPIC = "driver/mpu/temp";
const unsigned int WATCHDOG = 1000;
