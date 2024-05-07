#ifndef MOTOR_TEST_H
#define MOTOR_TEST_H

#include "../MotorController/MotorController.h"

// function prototypes
void Test_Individual_Motor(motor_info *motor);
void Test_Turn_Right(motor_info *motor_array);
void Test_Turn_Left(motor_info *motor_array);
void Test_Shift_Left(motor_info *motor_array);
#endif // MOTOR_TEST_H
