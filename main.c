#include <stdio.h>
#include "MotorController.h"
#include "MotorTest.h"

int main() {
    // initialize the motor information structures
    motor_info motor_a_args = {FORWARD, 100, PWMA, AIN1, AIN2, MOTORHAT_1};
    motor_info motor_b_args = {FORWARD, 100, PWMB, BIN1, BIN2, MOTORHAT_1};
    motor_info motor_c_args = {FORWARD, 100, PWMA, AIN1, AIN2, MOTORHAT_2};
    motor_info motor_d_args = {FORWARD, 100, PWMB, BIN1, BIN2, MOTORHAT_2};

    // shift the motors left
    printf("Shifting motors left...\n");
    Shift_Left(&motor_a_args, &motor_b_args, &motor_c_args, &motor_d_args);

    // stop the motors
    printf("Stopping motors...\n");
    Stop_Motor(&motor_a_args);
    Stop_Motor(&motor_b_args);
    Stop_Motor(&motor_c_args);
    Stop_Motor(&motor_d_args);
    
    testIndividualMotor(&motor_a_args);

    return 0;
}
