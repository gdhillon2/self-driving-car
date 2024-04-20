/****************************************************************
 * Class:: CSC-615-01 Spring 2024
 * Name:: Gurvir Dhillon
 * Student ID:: 923067350
 * Github-Name:: gdhillonSFSU
 * Project:: Assignment 3 - Motors
 * File:: assignment3.c
 *
 * Description:: This program utilizes the WaveShare Motor Driver
 * Hat and Raspberry Pi to start a motor with a button. It runs
 * at 100% power for 2 seconds, slows down to 15% gradually, then
 * stops for 1 second, and finally gradually builds up to 100%
 * power but in reverse.
 * **************************************************************/
#include "lib/Config/DEV_Config.h"
#include "lib/PCA9685/PCA9685.h"
#include "gpioheader.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define MOTOR_A 0
#define MOTOR_B 1

#define PWMA PCA_CHANNEL_0
#define AIN1 PCA_CHANNEL_1
#define AIN2 PCA_CHANNEL_2
#define PWMB PCA_CHANNEL_5
#define BIN1 PCA_CHANNEL_3
#define BIN2 PCA_CHANNEL_4

#define FORWARD 0
#define BACKWARD 1

#define MOTORHAT_1 0x40
#define MOTORHAT_2 0x51

typedef struct {
  unsigned int
      direction;  // this stores whether the wheel is going forward or backwards
  uint16_t speed; // stores what percentage of power we want to send via PWM

  uint8_t pwm;      // the PWM channel we are setting
  uint8_t forward;  // pca channel that helps determine direction
  uint8_t backward; // pca channel that helps determine direction

  uint8_t motorhat; // might need this later
} motor_info;

// this function sets the speed of the motor connected to the specified pwm
// channel
void Run_Motor(motor_info* motor) {

  // 100 is max speed
  if (motor->speed > 100) {
    motor->speed = 100;
  }

  printf("pwm %d speed %d direction %d\n", motor->pwm, motor->speed, motor->direction);
  PCA9685_SetPwmDutyCycle(motor->pwm, motor->speed);

  if (motor->direction == FORWARD) {
    // printf("forward\n");
    PCA9685_SetLevel(motor->forward, 0);
    PCA9685_SetLevel(motor->backward, 1);
  } else {
    // printf("backward\n");
    PCA9685_SetLevel(motor->forward, 1);
    PCA9685_SetLevel(motor->backward, 0);
  }
}

void Stop_Motor(motor_info* motor) {
  PCA9685_SetPwmDutyCycle(motor->pwm, 0);
  printf("stopping pwm channel %d\n", motor->pwm);
}

// this function should run the motor for 2 seconds at max power, then exit the
// function args contains a pointer to the motor_info struct
void *threaded_motor(void *args) {
  motor_info *motor = args;
  Run_Motor(motor);
  sleep(2);
  Stop_Motor(motor);
  return NULL;
}

int main() {

  if (DEV_ModuleInit())
    return 1;
  printf("dev config initialized\n");

  int BUTTON_GPIO = 17;

  // these functions (and the other gpio functions that sound similar
  // to PiGPIO library) are from gpioheader.h
  setup_io();
  set_gpio_input(BUTTON_GPIO);

  // memory address is currently hardcoded but we have macros defined at the top
  // that hopefully work
  PCA9685_Init(0x51);
  PCA9685_SetPWMFreq(100);

  printf("press the button to start the motor\n");

  // continuously loop inside this while loop until button is pressed,
  // once button is pressed we begin threading
  //while (!get_pin_value(BUTTON_GPIO)) {
    // do nothing
  //}

  // we can keep track of the motors by their index in this thread array
  // (this will become slightly problematic with a second motorhat but
  // we can always have another array for the second axle or figure
  // something else out)
  //pthread_t motor_threads[2];

  motor_info motor_a_args = {FORWARD, 100, PWMA, AIN1, AIN2, MOTORHAT_1};
  motor_info motor_b_args = {BACKWARD, 100, PWMB, BIN1, BIN2, MOTORHAT_1};

  Run_Motor(&motor_b_args);
  Run_Motor(&motor_a_args);
  sleep(2);
  Run_Motor(&motor_a_args);
  Stop_Motor(&motor_b_args);
  DEV_ModuleExit();

  return 0;
}
