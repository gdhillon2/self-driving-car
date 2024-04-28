#include "MotorController.h"
#include "lib/PCA9685/PCA9685.h"
#include <stdio.h>
#include <unistd.h>

void Init_Motorhat(uint8_t motorhat) {
    PCA9685_Init(motorhat);
    PCA9685_SetPWMFreq(100);
}

void Run_Motor(motor_info *motor) {
    Init_Motorhat(motor->motorhat);
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
    Init_Motorhat(motor->motorhat);
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

void Shift_Left(motor_info *motor_a, motor_info *motor_b, motor_info *motor_c, motor_info *motor_d) {

    Set_Direction(motor_a, BACKWARD);
    // Set_Direction(motor_b, FORWARD);
    Set_Direction(motor_c, BACKWARD);
    Set_Direction(motor_d, FORWARD);

    Run_Motor(motor_a);
    Run_Motor(motor_b);
    Run_Motor(motor_c);
    Run_Motor(motor_d);

    sleep(5); // Change later to dynamically stop the shift once object is not detected
}
