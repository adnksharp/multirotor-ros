#include <micro_ros_arduino.h>
#include <stdio.h>

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32.h>

#include "config.h"
//#include "utils.h"

rcl_publisher_t publisher;
std_msgs__msg__Int32 msg;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){err(LED);}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

void publish(rcl_timer_t * timer, int64_t last_call_time)
{  
	RCLC_UNUSED(last_call_time);
	if (timer != NULL) 
	{
		RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
		msg.data++;
	}
}

void err(byte pin)
{
	while (1)
	{
		digitalWrite(pin, !digitalRead(pin));
		delay(50);
	}
}

struct UROS {
	void spin()
	{
		RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
	}

	bool ping()
	{
		return rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)) == RCL_RET_OK;
	}

	void init()
	{
		set_microros_transports();
		allocator = rcl_get_default_allocator();

		rcl_init_options_t opts = rcl_get_zero_initialized_init_options();
		RCCHECK(rcl_init_options_init(&opts, allocator));
		RCCHECK(rcl_init_options_set_domain_id(&opts, DOMAIN_ID));
		RCCHECK(rclc_support_init_with_options(&support, 0, NULL, &opts, &allocator));
		RCCHECK(rclc_node_init_default(&node, NODE_NAME, "", &support));
		RCCHECK(rclc_publisher_init_default(&publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), TOPIC_NAME));
		RCCHECK(rclc_timer_init_default(&timer, &support, RCL_MS_TO_NS(WATCHDOG), publish));
		RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
		RCCHECK(rclc_executor_add_timer(&executor, &timer));
		msg.data = 0;
	}
};
