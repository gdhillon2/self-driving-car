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
  PCA9685_SetPwmDutyCycle(motor->pwm, motor->speed);

  if (motor->direction == FORWARD) {
    printf("forward\n");
    PCA9685_SetLevel(motor->IN1, 0);
    PCA9685_SetLevel(motor->IN2, 1);
  } else {
    printf("backward\n");
    PCA9685_SetLevel(motor->IN1, 1);
    PCA9685_SetLevel(motor->IN2, 0);
  }
  printf("Ran motor\n");
}

void Stop_Motor(motor_info *motor) {
  PCA9685_SetPwmDutyCycle(motor->pwm, 0);
  printf("stopping pwm channel %d\n", motor->pwm);
}

void Switch_Direction(motor_info *motor) {
  if (motor->direction == FORWARD) {
    motor->direction = BACKWARD;
  } else if (motor->direction == BACKWARD) {
    motor->direction = FORWARD;
  }
  printf("changed direction: %d\n", motor->direction);
}

void Set_Direction(motor_info *motor, unsigned int direction) {
  motor->direction = direction;
}

// void Shift_Left(motor_info *motor_a, motor_info motor_b, motor_info motor_c,
// motor_info motor_d) {
//
// }

void testIndividualMotor(motor_info *motor) {

  initMotorhat(motor->motorhat);

  Run_Motor(motor);

  sleep(4);

  Set_Direction(motor, BACKWARD);
  sleep(4);

  Stop_Motor(motor);
}

void testIndividualHat(uint8_t motorhat, motor_info *motor_a,
                       motor_info *motor_b) {
  initMotorhat(motorhat);

  Run_Motor(motor_a);
  Run_Motor(motor_b);

  sleep(5);

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

  sleep(5);

  // stop the motors on motorhat 2 (0x51)
  Stop_Motor(motor_a);
  Stop_Motor(motor_b);

  // init and stop the motors on motorhat 1 (0x40)
  initMotorhat(MOTORHAT_1);
  Stop_Motor(motor_a);
  Stop_Motor(motor_b);

  // both motors are stopped at this point

  Switch_Direction(motor_a);
  Switch_Direction(motor_b);
  Run_Motor(motor_a);
  Run_Motor(motor_b);

  // motor hat 1 is running in the opposite direction at this point

  initMotorhat(MOTORHAT_2);

  Run_Motor(motor_a);
  Run_Motor(motor_b);

  sleep(5);

  Stop_Motor(motor_a);
  Stop_Motor(motor_b);

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
  motor_info motor_b_args = {FORWARD, 100, PWMB, BIN1, BIN2, MOTORHAT_1};
  motor_info motor_c_args = {FORWARD, 100, PWMA, AIN1, AIN2, MOTORHAT_2};
  motor_info motor_d_args = {FORWARD, 100, PWMA, AIN1, AIN2, MOTORHAT_2};
  // TESTING MOTOR HAT 0x51
  // testIndividualHat(MOTORHAT_2, &motor_a_args, &motor_b_args);

  // TESTING MOTOR HAT 0x40
  // testIndividualHat(MOTORHAT_1, &motor_a_args, &motor_b_args);

  // TESTING BOTH MOTOR HATS
  // testBothHats(&motor_a_args, &motor_b_args);

  testIndividualMotor(&motor_a_args);
  return 0;
}
