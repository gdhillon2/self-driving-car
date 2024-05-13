#include "MotorController.h"
#include <stdio.h>
#include <unistd.h>

void Init_Motorhat(uint8_t motorhat) {
  PCA9685_Init(motorhat);
  PCA9685_SetPWMFreq(100);
}

void Init_Motor(motor_info *motor, unsigned int dir, uint16_t speed,
                uint8_t pwm, uint8_t IN1, uint8_t IN2, uint8_t motorhat) {
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

  PCA9685_SetPwmDutyCycle(motor->pwm, motor->speed);

  if (motor->direction == FORWARD) {
    PCA9685_SetLevel(motor->IN1, 0);
    PCA9685_SetLevel(motor->IN2, 1);
  } else {
    PCA9685_SetLevel(motor->IN1, 1);
    PCA9685_SetLevel(motor->IN2, 0);
  }
  DEV_ModuleExit();
}

void Move_All_Forward(motor_info *motor_array) {
  for (int i = 0; i < MOTOR_NUM; i++) {
    (&motor_array[i])->speed = 100;
    Set_Direction(&motor_array[i], FORWARD);
    Run_Motor(&motor_array[i]);
  }
}

void Move_All_Backward(motor_info *motor_array) {
  for (int i = 0; i < MOTOR_NUM; i++) {
    (&motor_array[i])->speed = 100;
    Set_Direction(&motor_array[i], BACKWARD);
    Run_Motor(&motor_array[i]);
  }
}
void Stop_Motor(motor_info *motor) {
  Init_Motorhat(motor->motorhat);
  PCA9685_SetPwmDutyCycle(motor->pwm, 0);
  DEV_ModuleExit();
}

void Stop_All_Motors(motor_info *motor_array) {
  for (int i = 0; i < MOTOR_NUM; i++) {
    Stop_Motor(&motor_array[i]);
  }
}

void Switch_Direction(motor_info *motor) {
  if (motor->direction == FORWARD) {
    motor->direction = BACKWARD;
  } else if (motor->direction == BACKWARD) {
    motor->direction = FORWARD;
  }
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
}

void Shift_Right(motor_info *motor_array) {

  Set_Direction(&motor_array[MOTOR_A], FORWARD);
  Set_Direction(&motor_array[MOTOR_B], BACKWARD);
  Set_Direction(&motor_array[MOTOR_C], FORWARD);
  Set_Direction(&motor_array[MOTOR_D], BACKWARD);

  Run_Motor(&motor_array[MOTOR_A]);
  Run_Motor(&motor_array[MOTOR_B]);
  Run_Motor(&motor_array[MOTOR_C]);
  Run_Motor(&motor_array[MOTOR_D]);
}

void Soft_Turn_Right(motor_info *motor_array) {
  for (int i = 0; i < MOTOR_NUM; i++) {
    Set_Direction(&motor_array[i], FORWARD);

    if (i == MOTOR_A || i == MOTOR_D) {
      (&motor_array[i])->speed = 100;
    } else
      (&motor_array[i])->speed = 40;
    Run_Motor(&motor_array[i]);
  }
}

void Turn_Right(motor_info *motor_array) {

  // sets direction of all motors to forward and sets power to 20% for motors
  // b and c
  for (int i = 0; i < MOTOR_NUM; i++) {
    Set_Direction(&motor_array[i], FORWARD);

    if (i == MOTOR_A || i == MOTOR_D)
      (&motor_array[i])->speed = 75;
    else {
      Set_Direction(&motor_array[i], BACKWARD);
      (&motor_array[i])->speed = 75;
    }
    Run_Motor(&motor_array[i]);
  }
}

void Soft_Turn_Left(motor_info *motor_array) {
  for (int i = 0; i < MOTOR_NUM; i++) {
    Set_Direction(&motor_array[i], FORWARD);

    if (i == MOTOR_A || i == MOTOR_D) {
      (&motor_array[i])->speed = 40;
    } else
      (&motor_array[i])->speed = 100;
    Run_Motor(&motor_array[i]);
  }
}

void Turn_Left(motor_info *motor_array) {
  for (int i = 0; i < MOTOR_NUM; i++) {
    Set_Direction(&motor_array[i], FORWARD);

    if (i == MOTOR_A || i == MOTOR_D) {
      Set_Direction(&motor_array[i], BACKWARD);
      (&motor_array[i])->speed = 75;
    } else
      (&motor_array[i])->speed = 75;
    Run_Motor(&motor_array[i]);
  }
}
