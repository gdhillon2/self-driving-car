#include "main.h"
#include "lib/MotorController/MotorController.h"
#include "lib/MotorTest/MotorTest.h"
#include "lib/SensorController/SensorController.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include "lib/GPIOHeader/GPIOHeader.h"
#include <pigpio.h>

volatile int running = 1;

// when the user presses ctrl + c, the threads
// fall out of their while loops and the program
// terminates
void sigintHandler() { running = 0; }

int main() {
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
//    for (int i = 0; i < MOTOR_NUM; i++) {
//      Run_Motor(&motors[i]);
//    }
////    sleep(5);
////    Shift_Left(motors);
////    sleep(5);
////    Shift_Right(motors);
////    sleep(5);
//    while (running) {
//      // do nothing
//    }
//
//    Stop_All_Motors(motors);
  /*****************************************************
   *  SENSOR TEST FUNCTIONS BELOW - UNCOMMENT AS NEEDED
   *****************************************************/
//  Running_Test(); // this function tests whether the running variable properly
                  // updates in another file when the signal handler is called
  sensor_info *sensors = Init_Sensors();

//  while(running) {
//  sensors[RIGHT_LINE_SENSOR].sensor_value = gpioRead(17);
//  printf("right line sensor\t%d\n", sensors[RIGHT_LINE_SENSOR].sensor_value);
//  sleep(1);
//  }

  pthread_t threads[SENSOR_NUM];

  // creates the IR and LINE threads and sends them the necessary structs
  if(pthread_create(&threads[RIGHT_LINE_SENSOR], NULL, Read_Sensor, (void *)&sensors[RIGHT_LINE_SENSOR])){
    printf("failed to create right line sensor thread\n");
    Free_Sensors(sensors);
    return 1;
  }

  if(pthread_create(&threads[LEFT_LINE_SENSOR], NULL, Read_Sensor, (void *)&sensors[LEFT_LINE_SENSOR])){
    printf("failed to create left line sensor thread\n");
    Free_Sensors(sensors);
    return 1;
  }

  signal(SIGINT, sigintHandler);

  while(running) {
    //printf("Right sensor: %d\tLeft sensor: %d\n", sensors[RIGHT_LINE_SENSOR].sensor_value, sensors[LEFT_LINE_SENSOR].sensor_value);
    if(sensors[LEFT_LINE_SENSOR].sensor_value && sensors[RIGHT_LINE_SENSOR].sensor_value){
      printf("\n**************************************BOTH SENSORS TRIPPED**************************************\n");
      //Move_All_Backward(motors);
      //break;
    }

    while(sensors[LEFT_LINE_SENSOR].sensor_value && !sensors[RIGHT_LINE_SENSOR].sensor_value) {
      Turn_Left(motors);
      usleep(500);
    }

    while(sensors[RIGHT_LINE_SENSOR].sensor_value && !sensors[LEFT_LINE_SENSOR].sensor_value) {
      Turn_Right(motors);
      usleep(500);
    }

    Move_All_Forward(motors);
  }

  if(pthread_join(threads[RIGHT_LINE_SENSOR], NULL)) {
    printf("failed to join right line sensor\n");
    Free_Sensors(sensors);
    return 1;
  }
  
  if(pthread_join(threads[LEFT_LINE_SENSOR], NULL)) {
    printf("failed to join left line sensor\n");
    Free_Sensors(sensors);
    return 1;
  }
  Stop_All_Motors(motors);
  printf("\nshutting down\n");
  free(motors);
  Free_Sensors(sensors);
  return 0;
}
