#pragma once

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/string.h>

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#endif
//https://icircuit.net/esp-idf-lets-control-servo-using-esp32/2073

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Aborting.\n",__LINE__,(int)temp_rc); vTaskDelete(NULL);}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Continuing.\n",__LINE__,(int)temp_rc);}}

#define SERVO_MIN_DUTY  1638 // duty cycle for 0 grades
#define SERVO_MAX_DUTY  3277 // duty cycle for 180 grades

typedef struct {
    uint8_t is_initialized;
    uint8_t freq_hz;
    uint8_t duty_resolution;
} TimerState;

typedef struct {
    uint8_t is_initialized;
    uint8_t timer;
    uint8_t channel;
    uint8_t gpio_num;
    uint16_t angle;
} ServoState;

typedef struct {
  ServoState horizontal_servo;
  ServoState vertical_servo; 
} PanTiltState;

TimerState timers[4] = {0};
PanTiltState pan_tilt_state = {0};

void pan_tilt_init(void);
void set_horizontal_angle(int16_t angle);
void set_vertical_angle(int16_t angle);

static void init_horizontal_servo(uint8_t channel_selected, uint8_t timer_selected, uint8_t gpio_num);
static void init_vertical_servo(uint8_t channel_selected, uint8_t timer_selected, uint8_t gpio_num);
static void init_pwm_timer(uint8_t timer_selected, uint8_t timer_freq_hz);
static void configure_timer(uint8_t timer_selected, uint8_t timer_freq_hz);
static void configure_servo(uint8_t channel_selected, uint8_t timer_selected, uint8_t gpio_num);
static void update_timer_state(uint8_t timer_selected, uint8_t timer_freq_hz);
static void update_horizontal_servo_state(uint8_t channel_selected, uint8_t timer_selected, uint8_t gpio_num);
static void update_vertical_servo_state(uint8_t channel_selected, uint8_t timer_selected, uint8_t gpio_num);