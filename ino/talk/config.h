#include <micro_ros_arduino.h>
#include <stdio.h>

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32.h>

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){err(LED);}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

#define LED 2
#define DOMAIN_ID 49

char *NODE_NAME = "tester";
char *PUB_TOPIC = "driver/talker";
char *SUB_TOPIC = "driver/listener";

const unsigned int WATCHDOG = 1000;
