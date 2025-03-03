#include "config.h"
//#include "utils.h"

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
		#if __has_include("wificfg.h")
			#include "wificfg.h"
			set_microros_wifi_transports(ssid, pass, host, port);
		#else
			set_microros_transports();
		#endif
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
