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
#include "DEV_Config.h"
#include "PCA9685.h"
#include "gpioheader.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define MOTOR_A 0
#define MOTOR_B 1

#define PWMA 0
#define AIN1 1
#define AIN2 2
#define PWMB 5
#define BIN1 3
#define BIN2 4

#define FORWARD 0
#define BACKWARD 1

#define MOTORHAT_1 0x40
#define MOTORHAT_2 0x51

typedef struct {
  unsigned int direction;
  uint16_t speed;
  
  uint8_t pwm;
  uint8_t forward;
  uint8_t backward;
  
  uint8_t motorhat; // might need this later
} motor_info;

void Run_Motor(uint8_t pwm, uint8_t forward, uint8_t backward, unsigned int direction, uint16_t speed) {

  // 100 is max speed
  if (speed > 100) {
    speed = 100;
  }

  printf("pwm %d speed %d direction %d\n", pwm, speed, direction);
  PCA9685_SetPwmDutyCycle(pwm, speed);

  if (direction == FORWARD) {
    // printf("forward\n");
    PCA9685_SetLevel(forward, 0);
    PCA9685_SetLevel(backward, 1);
  } else {
    // printf("backward\n");
    PCA9685_SetLevel(forward, 1);
    PCA9685_SetLevel(backward, 0);
  }
}

void Stop_Motor(uint8_t pwm) { 
  PCA9685_SetPwmDutyCycle(pwm, 0);
  printf("stopping pwm channel %d\n", pwm);
}

void *threaded_motor(void *args){
  motor_info *motor = args;
  Run_Motor(motor->pwm, motor->forward, motor->backward, motor->direction, motor->speed);
  sleep(2);
  Stop_Motor(motor->pwm);
  return NULL;
}

int main() {

  if (DEV_ModuleInit())
    return 1;
  else
    printf("dev config initialized\n");

  int BUTTON_GPIO = 17;

  setup_io();
  set_gpio_input(BUTTON_GPIO);

  PCA9685_Init(0x40);
  PCA9685_SetPWMFreq(100);
  
  if (!get_pin_value(BUTTON_GPIO))
    printf("press the button to start the motor\n");

//  while (!get_pin_value(BUTTON_GPIO)) {
//    // do nothing
//  }
  // this function initializes the memory location for the PCA9685 device

  pthread_t threads[2];

  motor_info motor_a_args = {FORWARD, 100, PWMA, AIN1, AIN2, MOTORHAT_1};
  motor_info motor_b_args = {BACKWARD, 100, PWMB, BIN1, BIN2, MOTORHAT_1};

  // this function calls runs the motor at max speed in the forward direction
  // the motor runs at max speed for 2 seconds
  if(pthread_create(&threads[MOTOR_A], NULL, threaded_motor, (void *)&motor_a_args) != 0){
    printf("failed to create thread for MOTOR A\n");
    return 1;
  }

  if(pthread_create(&threads[MOTOR_B], NULL, threaded_motor, (void *)&motor_b_args) != 0){
    printf("failed to create thread for MOTOR A\n");
    return 1;
  }

  if(pthread_join(threads[MOTOR_A], NULL)){
    printf("failed to join thread for MOTOR A\n");
    return 1;
  }

  if(pthread_join(threads[MOTOR_B], NULL)){
    printf("failed to join thread for MOTOR A\n");
    return 1;
  }

  DEV_ModuleExit();

  return 0;
}
