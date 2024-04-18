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

#define MOTOR_A 0
#define MOTOR_B 1

void Run_Motor(unsigned int motor, unsigned int direction, uint16_t speed) {

  // 100 is max speed
  if (speed > 100) {
    speed = 100;
  }

  printf("speed %d ", speed);
  PCA9685_SetPwmDutyCycle(motor, speed); // 0 is motor a

  if (direction == 1) {
    printf("forward\n");
    PCA9685_SetLevel(1, 0); // 1 is AIN1
    PCA9685_SetLevel(2, 1); // 2 is AIN2
  } else {
    printf("backward\n");
    PCA9685_SetLevel(1, 1);
    PCA9685_SetLevel(2, 0);
  }
}

void Stop_Motor(unsigned int motor) { PCA9685_SetPwmDutyCycle(motor, 0); }

int main() {

  if (DEV_ModuleInit())
    return 1;
  else
    printf("dev config initialized\n");

  int BUTTON_GPIO = 21;

  setup_io();
  set_gpio_input(BUTTON_GPIO);

  if (!get_pin_value(BUTTON_GPIO))
    printf("press the button to start the motor\n");

  while (!get_pin_value(BUTTON_GPIO)) {
    // do nothing
  }
  // this function initializes the memory location for the PCA9685 device
  PCA9685_Init(0x40);
  PCA9685_SetPWMFreq(100);

  uint16_t speed = 100;

  // this function calls runs the motor at max speed in the forward direction
  // the motor runs at max speed for 2 seconds
  Run_Motor(MOTOR_A, 1, speed);
  sleep(2);

  // this loop decreases power going into the motor (going forward) gradually
  // until it is at 15% power, the total time this loop takes is 2.55 seconds
  while (speed >= 15) {
    Run_Motor(MOTOR_A, 1, speed--);
    usleep(30000);
  }

  // stop motor for 1 second
  Stop_Motor(MOTOR_A);
  sleep(1);

  speed = 0;

  // this loop increases power going into the motor (going backward) gradually
  // until max power, total time for this loop is 3 seconds
  while (speed <= 100) {
    Run_Motor(MOTOR_A, 2, speed++);
    usleep(30000);
  }

  sleep(1);
  Stop_Motor(MOTOR_A);
  DEV_ModuleExit();

  return 0;
}
