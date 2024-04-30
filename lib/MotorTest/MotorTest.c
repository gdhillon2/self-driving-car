#include "MotorTest.h"

void Test_Individual_Motor(motor_info *motor) {

  Run_Motor(motor);

  sleep(4);

  Set_Direction(motor, BACKWARD);
  Run_Motor(motor);
  sleep(4);

  Stop_Motor(motor);
}

void Test_Turn_Right(motor_info *motor_array) {
  Turn_Right(motor_array);
  sleep(4);
  Stop_All_Motors(motor_array);
}

void Test_Turn_Left(motor_info *motor_array) {
  Turn_Left(motor_array);
  sleep(4);
  Stop_All_Motors(motor_array);
}

void Test_Shift_Left(motor_info *motor_array) {
  Shift_Left(motor_array);
  sleep(4);
  Stop_All_Motors(motor_array);
}
