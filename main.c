#include "main.h"
#include "lib/MotorController/MotorController.h"
// when the user presses ctrl + c, the threads
// fall out of their while loops and the program
// terminates
void sigintHandler(int sig) {
  running = 0;
  printf("\nshutting down\n");
}

int main() {
  signal(SIGINT, sigintHandler);
  printf("press ctrl+c to shut down\n");
  // initialize the motor information structures
  motor_info *motors = malloc(sizeof(motor_info) * MOTOR_NUM);

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
  //  Test_Turn_Right(motors);
  //  Test_Turn_Left(motors);
  //  Test_Shift_Left(motors);

  /*****************************************************
   *  TESTS SIGNAL HANDLER
   *****************************************************/
  for (int i = 0; i < MOTOR_NUM; i++) {
    Run_Motor(&motors[i]);
  }
  while (running) {
    // do nothing
  }

  Stop_All_Motors(motors);

  /*****************************************************
   *  SENSOR TEST FUNCTIONS BELOW - UNCOMMENT AS NEEDED
   *****************************************************/

  free(motors);
  return 0;
}