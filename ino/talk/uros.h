/*
void subscription_callback(const void * msgin)
{
	const std_msgs__msg__Int32 * called = (const std_msgs__msg__Int32 *)msgin;
	digitalWrite(LED, called->data == 1);
}
*/

struct UROS 
{
    rcl_subscription_t subscriber;
    rcl_publisher_t mpu_temp;
    rcl_publisher_t mpu_accel;
    rcl_publisher_t mpu_gyro;
    std_msgs__msg__Float32MultiArray multi_f32_msg;
    float* multi_f32_data;
    rclc_executor_t executor;
    rclc_support_t support;
    rcl_allocator_t allocator;
    rcl_node_t node;
    rcl_timer_t timer;
    size_t multi_f32_capacity;

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
    }

    void create_f32(const char * topic_name, rcl_publisher_t & publisher_f32)
	{
        std_msgs__msg__Float32 msg;
        RCCHECK(rclc_publisher_init_default(
            &publisher_f32,
            &node,
            ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
            topic_name
        ));
    }

    void create_multi_f32(const char * topic_name, size_t capacity, rcl_publisher_t & publisher_multi_f32)
	{
        std_msgs__msg__Float32MultiArray msg;
        RCCHECK(rclc_publisher_init_default(
            &publisher_multi_f32,
            &node,
            ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32MultiArray),
            topic_name
        ));

        std_msgs__msg__Float32MultiArray__init(&multi_f32_msg);
        multi_f32_capacity = capacity;
        multi_f32_data = (float*)malloc(sizeof(float) * capacity);
        multi_f32_msg.data.capacity = capacity;
        multi_f32_msg.data.data = multi_f32_data;
    }

    void spin_publisher_f32(float data, rcl_publisher_t & publisher_f32)
	{
        std_msgs__msg__Float32 msg;
        msg.data = data;
        RCSOFTCHECK(rcl_publish(&publisher_f32, &msg, NULL));
    }

    void spin_publisher_multi_f32(float data[], size_t size, rcl_publisher_t & publisher_multi_f32)
	{
        if (size > multi_f32_capacity) {
            return;
        }

        multi_f32_msg.data.size = size;
        for (size_t i = 0; i < size; ++i) {
            multi_f32_data[i] = data[i];
        }

        RCSOFTCHECK(rcl_publish(&publisher_multi_f32, &multi_f32_msg, NULL));
    }

    void spin_subscriber() 
	{
        RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
    }
};
