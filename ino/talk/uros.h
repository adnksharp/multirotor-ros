#include "config.h"

void subscription_callback(const void * msgin)
{
	const std_msgs__msg__Int32 * called = (const std_msgs__msg__Int32 *)msgin;
	digitalWrite(LED, called->data == 1);
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
	rcl_subscription_t subscriber;
	rcl_publisher_t publisher;
	//std_msgs__msg__Int32 msg;
	std_msgs__msg__Int32 talk_msg;
	std_msgs__msg__Int32 listen_msg;
	rclc_executor_t executor;
	rclc_support_t support;
	rcl_allocator_t allocator;
	rcl_node_t node;
	rcl_timer_t timer;

	void begin()
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
		
		talk_msg.data = 0;
		listen_msg.data = 0;
	}

	void create_publisher(const char * topic_name)
	{
		std_msgs__msg__Int32 * msg = &talk_msg;
		RCCHECK(rclc_publisher_init_best_effort(
			&publisher, 
			&node, 
			ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), 
			topic_name
		));
	}

	void create_subscriber(const char * topic_name)
	{
		RCCHECK(rclc_subscription_init_default(
			&subscriber, 
			&node, 
			ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), 
			topic_name
		));

		RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
		RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &listen_msg, &subscription_callback, ON_NEW_DATA));
	}

	void spin_publisher()
	{
		RCSOFTCHECK(rcl_publish(&publisher, &talk_msg, NULL));
		talk_msg.data++;
	}

	void spin_subscriber()
	{
		RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
	}
};
