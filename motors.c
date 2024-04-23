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
#include "gpioheader.h"
#include "lib/Config/DEV_Config.h"
#include "lib/PCA9685/PCA9685.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

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

  uint8_t pwm; // the PWM channel we are setting
  uint8_t IN1; // pca channel that helps determine direction
  uint8_t IN2; // pca channel that helps determine direction

  uint8_t motorhat; // might need this later
} motor_info;

void initMotorhat(uint8_t motorhat) {
  PCA9685_Init(motorhat);
  PCA9685_SetPWMFreq(100);
}

// this function sets the speed of the motor connected to the specified pwm
// channel
void Run_Motor(motor_info *motor) {

  // 100 is max speed
  if (motor->speed > 100) {
    motor->speed = 100;
  }

  printf("pwm %d speed %d direction ", motor->pwm, motor->speed);
  if (motor->direction == FORWARD)
    printf("forward\n");
  else
    printf("backward\n");
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

void Stop_Motor(motor_info *motor) {
  PCA9685_SetPwmDutyCycle(motor->pwm, 0);
  printf("stopping pwm channel %d\n", motor->pwm);
}

void testIndividualHat(uint8_t motorhat, motor_info *motor_a,
                       motor_info *motor_b) {
  initMotorhat(motorhat);

  Run_Motor(motor_a);
  Run_Motor(motor_b);
  sleep(2);
  Stop_Motor(motor_a);
  Stop_Motor(motor_b);
  DEV_ModuleExit();
}

void testBothHats(motor_info *motor_a, motor_info *motor_b) {
  // init and begin running the motors on motorhat 1 (0x40)
  initMotorhat(MOTORHAT_1);
  Run_Motor(motor_a);
  Run_Motor(motor_b);

  // init and begin running the motors on motorhat 2 (0x51)
  initMotorhat(MOTORHAT_2);
  Run_Motor(motor_a);
  Run_Motor(motor_b);

  sleep(2);

  // stop the motors on motorhat 2 (0x51)
  Stop_Motor(motor_a);
  Stop_Motor(motor_b);

  // init and stop the motors on motorhat 1 (0x40)
  initMotorhat(MOTORHAT_1);
  Stop_Motor(motor_a);
  Stop_Motor(motor_b);

  DEV_ModuleExit();
}

int main() {

  if (DEV_ModuleInit())
    return 1;
  printf("dev config initialized\n");

  motor_info motor_a_args = {FORWARD, 100, PWMA, AIN1, AIN2, MOTORHAT_1};
  motor_info motor_b_args = {BACKWARD, 100, PWMB, BIN1, BIN2, MOTORHAT_1};

  // TESTING MOTOR HAT 0x51
  testIndividualHat(MOTORHAT_2, &motor_a_args, &motor_b_args);

  // TESTING MOTOR HAT 0x40
  testIndividualHat(MOTORHAT_1, &motor_a_args, &motor_b_args);

  // TESTING BOTH MOTOR HATS
  testBothHats(&motor_a_args, &motor_b_args);

  return 0;
}
