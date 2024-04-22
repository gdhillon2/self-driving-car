/****************************************************************
 * Class:: CSC-615-01 Spring 2024
 * Name:: Gurvir Dhillon
 * Student ID:: 923067350
 * Github-Name::
 * Project::
 * File:: motors.c
 *
 * Description::
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

// macros for motor direction
#define FORWARD 0
#define BACKWARD 1

// macros that define the address of each motor hat
// each motor hat refers to an axle controlling 2 motors
#define MOTORHAT_1 0x40
#define MOTORHAT_2 0x51

typedef struct {
  unsigned int
      direction;  // this stores whether the wheel is going forward or backwards
  uint16_t speed; // stores what percentage of power we want to send via PWM

  uint8_t pwm;      // the PWM channel we are setting
  uint8_t IN1;  // pca channel that helps determine direction
  uint8_t IN2; // pca channel that helps determine direction

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
    PCA9685_SetLevel(motor->IN1, 0);
    PCA9685_SetLevel(motor->IN2, 1);
  } else {
    // printf("backward\n");
    PCA9685_SetLevel(motor->IN1, 1);
    PCA9685_SetLevel(motor->IN2, 0);
  }
}

void Stop_Motor(motor_info* motor) {
  PCA9685_SetPwmDutyCycle(motor->pwm, 0);
  printf("stopping pwm channel %d\n", motor->pwm);
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
  PCA9685_Init(MOTORHAT_2);
  PCA9685_SetPWMFreq(100);

  printf("press the button to start the motor\n");

  // continuously loop inside this while loop until button is pressed,
  // once button is pressed we begin threading
  //while (!get_pin_value(BUTTON_GPIO)) {
    // do nothing
  //}

  motor_info motor_a_args = {FORWARD, 100, PWMA, AIN1, AIN2, MOTORHAT_1};
  motor_info motor_b_args = {BACKWARD, 100, PWMB, BIN1, BIN2, MOTORHAT_1};

  Run_Motor(&motor_b_args);
  Run_Motor(&motor_a_args);
  sleep(2);
  Stop_Motor(&motor_b_args);
  Stop_Motor(&motor_a_args);
  DEV_ModuleExit();

  return 0;
}
