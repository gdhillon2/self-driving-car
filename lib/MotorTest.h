#ifndef MOTOR_TEST_H
#define MOTOR_TEST_H

#include "MotorController.h"

// function prototypes
void testIndividualMotor(motor_info *motor);
void testMotors(motor_info *motor_a, motor_info *motor_b, motor_info *motor_c, motor_info *motor_d);
void testIndividualHat(uint8_t motorhat, motor_info *motor_a, motor_info *motor_b);
void testBothHats(motor_info *motor_a, motor_info *motor_b);

#endif // MOTOR_TEST_H
