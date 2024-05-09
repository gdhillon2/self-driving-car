#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include "../PCA9685/PCA9685.h"
#include <stdint.h>

// macros for motor direction
#define FORWARD 0
#define BACKWARD 1

// macros for PCA channels
#define PWMA PCA_CHANNEL_0
#define AIN1 PCA_CHANNEL_1
#define AIN2 PCA_CHANNEL_2
#define PWMB PCA_CHANNEL_5
#define BIN1 PCA_CHANNEL_3
#define BIN2 PCA_CHANNEL_4

// macros for motorhats
#define MOTORHAT_1 0x40
#define MOTORHAT_2 0x51

// macros for the number of motors and which motor they are
#define MOTOR_NUM 4

#define MOTOR_A 0
#define MOTOR_B 1
#define MOTOR_C 2
#define MOTOR_D 3

// motor information structure
typedef struct {
  unsigned int
      direction;  // this stores whether the wheel is going forward or backwards
  uint16_t speed; // stores what percentage of power we want to send via PWM
  uint8_t pwm;    // the PWM channel we are setting
  uint8_t IN1;    // pca channel that helps determine direction
  uint8_t IN2;    // pca channel that helps determine direction
  uint8_t motorhat; // stores what motorhat the motor belongs to
} motor_info;

// function prototypes
void Init_Motorhat(uint8_t motorhat);
void Init_Motor(motor_info *motor, unsigned int dir, uint16_t speed,
                uint8_t pwm, uint8_t IN1, uint8_t IN2, uint8_t motorhat);
void Run_Motor(motor_info *motor);
void Stop_Motor(motor_info *motor);
void Move_All_Motors(motor_info *motor);
void Move_All_Forward(motor_info *motor_array);
void Move_All_Backward(motor_info *motor_array);
void Stop_All_Motors(motor_info *motor_array);
void Switch_Direction(motor_info *motor);
void Set_Direction(motor_info *motor, unsigned int direction);
void Shift_Left(motor_info *motor_array);
void Shift_Right(motor_info *motor_array);
void Turn_Right(motor_info *motor_array);
void Turn_Left(motor_info *motor_array);
#endif // MOTOR_CONTROLLER_H
