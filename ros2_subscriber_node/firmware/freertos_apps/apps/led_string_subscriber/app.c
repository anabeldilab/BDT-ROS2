#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/string.h>

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#endif

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Aborting.\n",__LINE__,(int)temp_rc); vTaskDelete(NULL);}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Continuing.\n",__LINE__,(int)temp_rc);}}

#define GPIO_LED 2
#define GPIO_LED_PIN_SEL (1ULL<< GPIO_LED)

// Set maximum received string length.
// Received publications with strings larger than this size will be discarded.
#define STRING_LEN 200

rcl_subscription_t subscriber;
std_msgs__msg__String msg;

static void init_hw(void)
{
gpio_config_t io_conf;
io_conf.mode=GPIO_MODE_OUTPUT;
io_conf.pin_bit_mask=GPIO_LED_PIN_SEL;
io_conf.intr_type=GPIO_INTR_DISABLE;
io_conf.pull_down_en=0;
io_conf.pull_up_en=0;
gpio_config(&io_conf);
}

static bool led_state=false;

void subscription_callback(const void * msgin)
{
	const std_msgs__msg__String * msg = (const std_msgs__msg__String *)msgin;
	printf("I have heard: \"%s\"\n", msg->data.data);
	if (strcmp(msg->data.data, "Up") == 0) {
		led_state = !led_state;
		gpio_set_level(GPIO_LED, led_state);
	}
}

void appMain(void * arg)
{
  rcl_allocator_t allocator = rcl_get_default_allocator();
	rclc_support_t support;

	// create init_options
	// Here is differente than the string_subscriber original
	RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

	// create node
	rcl_node_t node;
	RCCHECK(rclc_node_init_default(&node, "dir_goal_RosSubscriber", "", &support));

	// create subscriber
	RCCHECK(rclc_subscription_init_default(
		&subscriber,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
		"/dir_goal"));

	// create executor
	rclc_executor_t executor;
	RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));

	// add subscriber callback to the executor
	RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msg, &subscription_callback, ON_NEW_DATA));

	//mod
	// Al final de toda la inicializacion de ros
	// hago la inicializacion del hardware
	init_hw();
	// end mod

	// Initialize subscriber message memory.
	char string_memory[STRING_LEN];
	msg.data.data = &string_memory[0];
	msg.data.size = 0;
	msg.data.capacity = STRING_LEN; 

	while(1) {
			rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
			usleep(100000);
	}

	// free resources
	RCCHECK(rclc_executor_fini(&executor))
	RCCHECK(rcl_subscription_fini(&subscriber, &node));
	RCCHECK(rcl_node_fini(&node));
	RCCHECK(rclc_support_fini(&support));

	
	// Here is differente than ping_pong version
	vTaskDelete(NULL);
}
