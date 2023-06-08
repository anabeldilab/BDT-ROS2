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
#include "driver/ledc.h"
#endif
//https://icircuit.net/esp-idf-lets-control-servo-using-esp32/2073

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Aborting.\n",__LINE__,(int)temp_rc); vTaskDelete(NULL);}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Continuing.\n",__LINE__,(int)temp_rc);}}

#define GPIO_LED 2
#define GPIO_LED_PIN_SEL (1ULL<< GPIO_LED)

// Set maximum received string length.
// Received publications with strings larger than this size will be discarded.
#define STRING_LEN 200
#define LENGTHMSGCOMMAND 6

rcl_subscription_t subscriber;
rcl_publisher_t publisher;
std_msgs__msg__String msgSubscriber;
std_msgs__msg__String msgPublisher;

const int bitSize = 15;
const int minValue = 1000;  // micro seconds (uS)
const int maxValue = 2000; // micro seconds (uS)
int verticalDuty = (1<<bitSize) * minValue / 20000;
int horizontalDuty = (1<<bitSize) * minValue / 20000;
// el duty es el valor que se le pasa al pwm y es el que determina el angulo del servo
// el valor minimo es 500 uS 
// el periodo es de 20 ms (50 Hz)
// el duty se calcula asi (1<<bitSize) * minValue / 20000 
// Donde (1<<bitSize) es: 1 desplazado bitSize veces a la izquierda (2^bitSize) 
// y 20000 es el periodo en micro segundos (20 ms)

static void init_hw(void)
{
	// init pwm timer 0
	ledc_timer_config_t timer_conf;
	timer_conf.bit_num = LEDC_TIMER_15_BIT;
	timer_conf.freq_hz = 50;
	timer_conf.speed_mode = LEDC_HIGH_SPEED_MODE;
	timer_conf.timer_num = LEDC_TIMER_0;
	ledc_timer_config(&timer_conf);

	// init pwm channel 0
	ledc_channel_config_t ledc_conf1;
	ledc_conf1.channel = LEDC_CHANNEL_0;
	ledc_conf1.duty = horizontalDuty;
	ledc_conf1.gpio_num = 17;
	ledc_conf1.intr_type = LEDC_INTR_DISABLE;
	ledc_conf1.speed_mode = LEDC_HIGH_SPEED_MODE;
	ledc_conf1.timer_sel = LEDC_TIMER_0;
	ledc_channel_config(&ledc_conf1);


  // init pwm servo channel 1
	ledc_channel_config_t ledc_conf2;
	ledc_conf2.channel = LEDC_CHANNEL_1;
	ledc_conf2.duty = verticalDuty;
	ledc_conf2.gpio_num = 16;
	ledc_conf2.intr_type = LEDC_INTR_DISABLE;
	ledc_conf2.speed_mode = LEDC_HIGH_SPEED_MODE;
	ledc_conf2.timer_sel = LEDC_TIMER_0;
	ledc_channel_config(&ledc_conf2);
}

void subscription_callback(const void * msgin)
{
	const std_msgs__msg__String * msg = (const std_msgs__msg__String *)msgin;

	char read_command[LENGTHMSGCOMMAND];

	for(int i=0;i<LENGTHMSGCOMMAND;i++)
		read_command[i]=msg->data.data[i];

	// Ponemos el mensaje en la direccion de memoria del mensaje del publisher
	msgPublisher.data.data = read_command;
	msgPublisher.data.size = LENGTHMSGCOMMAND;
	msgPublisher.data.capacity = LENGTHMSGCOMMAND;

	if (strcmp(msg->data.data, "Up") == 0) {
		verticalDuty -= 200;
		RCSOFTCHECK(rcl_publish(&publisher, &msgPublisher, NULL));
	} else if (strcmp(msg->data.data, "Down") == 0) {
		verticalDuty += 200;
		RCSOFTCHECK(rcl_publish(&publisher, &msgPublisher, NULL));
	} else if (strcmp(msg->data.data, "Left") == 0) {
		horizontalDuty += 200;
		RCSOFTCHECK(rcl_publish(&publisher, &msgPublisher, NULL));
	} else if (strcmp(msg->data.data, "Right") == 0) {
		horizontalDuty -= 200;
		RCSOFTCHECK(rcl_publish(&publisher, &msgPublisher, NULL));
	}

	if (verticalDuty < minValue) {
		verticalDuty = minValue;
	} else if (verticalDuty > maxValue) {
		verticalDuty = maxValue;
	}

	if (horizontalDuty < minValue) {
		horizontalDuty = minValue;
	} else if (horizontalDuty > maxValue) {
		horizontalDuty = maxValue;
	}

	RCSOFTCHECK(rcl_publish(&publisher, &msgPublisher, NULL));

	ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, horizontalDuty);
	ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);	

	ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, verticalDuty);
	ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);
}

void appMain(void * arg)
{
  rcl_allocator_t allocator = rcl_get_default_allocator();
	rclc_support_t support;

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

	// create publisher
	RCCHECK(rclc_publisher_init_default(
		&publisher,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
		"/debug_topic"));

	// create executor
	rclc_executor_t executor;
	RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));

	// add subscriber callback to the executor
	RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msgSubscriber, &subscription_callback, ON_NEW_DATA))

	//mod
	// Al final de toda la inicializacion de ros
	// hago la inicializacion del hardware
	init_hw();
	// end mod

	// Initialize subscriber message memory
	char string_subscriber_memory[STRING_LEN];
	msgSubscriber.data.data = &string_subscriber_memory[0];
	msgSubscriber.data.size = 0;
	msgSubscriber.data.capacity = STRING_LEN;

	char string_publisher_memory[STRING_LEN];
	msgPublisher.data.data = &string_publisher_memory[0];
	msgPublisher.data.size = (unsigned)20;
	msgPublisher.data.capacity = STRING_LEN;

	while(1) {
		rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
		usleep(100000);
	}

	// free resources
	RCCHECK(rclc_executor_fini(&executor))
	RCCHECK(rcl_subscription_fini(&subscriber, &node));
	RCCHECK(rcl_publisher_fini(&publisher, &node));
	RCCHECK(rcl_node_fini(&node));
	RCCHECK(rclc_support_fini(&support));

	vTaskDelete(NULL);
}
