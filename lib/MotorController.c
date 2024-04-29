#include "MotorController.h"
#include "PCA9685/PCA9685.h"
#include <stdio.h>
#include <unistd.h>

void Init_Motorhat(uint8_t motorhat) {
    PCA9685_Init(motorhat);
    PCA9685_SetPWMFreq(100);
}

void Init_Motor(motor_info *motor, unsigned int dir, uint16_t speed, uint8_t pwm, uint8_t IN1, uint8_t IN2, uint8_t motorhat) {
    motor->direction = dir;
    motor->speed = speed;
    motor->pwm = pwm;
    motor->IN1 = IN1;
    motor->IN2 = IN2;
    motor->motorhat = motorhat;
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

void Stop_All_Motors(motor_info *motor_array) {
  for(int i = 0; i < MOTOR_NUM; i++) {
    Stop_Motor(&motor_array[i]);
  }
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

void Shift_Left(motor_info *motor_array) {

    Set_Direction(&motor_array[MOTOR_A], BACKWARD);
    Set_Direction(&motor_array[MOTOR_B], FORWARD);
    Set_Direction(&motor_array[MOTOR_C], BACKWARD);
    Set_Direction(&motor_array[MOTOR_D], FORWARD);

    Run_Motor(&motor_array[MOTOR_A]);
    Run_Motor(&motor_array[MOTOR_B]);
    Run_Motor(&motor_array[MOTOR_C]);
    Run_Motor(&motor_array[MOTOR_D]);

    sleep(5); // Change later to dynamically stop the shift once object is not detected
}

void Turn_Right(motor_info *motor_array){
  
  // sets direction of all motors to forward and sets power to 20% for motors b and c
  for(int i = 0; i < MOTOR_NUM; i++) {
    Set_Direction(&motor_array[i], FORWARD);

    if(i == MOTOR_A || i == MOTOR_D)
      (&motor_array[i])->speed = 100;
    else
      (&motor_array[i])->speed = 20;

    Run_Motor(&motor_array[i]);
  }
}

void Turn_Left(motor_info *motor_array){
  
  // sets direction of all motors to forward and sets power to 20% for motors a and d
  for(int i = 0; i < MOTOR_NUM; i++) {
    Set_Direction(&motor_array[i], FORWARD);

    if(i == MOTOR_A || i == MOTOR_D)
      (&motor_array[i])->speed = 20;
    else
      (&motor_array[i])->speed = 100;

    Run_Motor(&motor_array[i]);
  }
}
