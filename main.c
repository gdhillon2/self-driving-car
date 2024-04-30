#include "lib/MotorController/MotorController.h"
#include "lib/MotorTest/MotorTest.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  // initialize the motor information structures
  motor_info* motors = malloc(sizeof(motor_info) * MOTOR_NUM);
  
  Init_Motor(&motors[MOTOR_A], FORWARD, 100, PWMA, AIN1, AIN2, MOTORHAT_1);
  Init_Motor(&motors[MOTOR_B], FORWARD, 100, PWMB, BIN1, BIN2, MOTORHAT_1);
  Init_Motor(&motors[MOTOR_C], FORWARD, 100, PWMA, AIN1, AIN2, MOTORHAT_2);
  Init_Motor(&motors[MOTOR_D], FORWARD, 100, PWMB, BIN1, BIN2, MOTORHAT_2);

/*****************************************************
*  MOTOR TEST FUNCTIONS BELOW - UNCOMMENT AS NEEDED
*****************************************************/

//  Test_Individual_Motor(&motors[MOTOR_A]);
//  Test_Individual_Motor(&motors[MOTOR_B]);
//  Test_Individual_Motor(&motors[MOTOR_C]);
//  Test_Individual_Motor(&motors[MOTOR_D]);
  Test_Turn_Right(motors);
  Test_Turn_Left(motors);
//  Test_Shift_Left(motors);

/*****************************************************
*  SENSOR TEST FUNCTIONS BELOW - UNCOMMENT AS NEEDED
*****************************************************/





  free(motors);  
  return 0;
}
