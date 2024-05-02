#include "main.h"
#include "lib/MotorController/MotorController.h"
#include "lib/MotorTest/MotorTest.h"
#include "lib/SensorController/SensorController.h"
#include <pigpio.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

volatile sig_atomic_t running = 1;

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

  // initialize the sensor structs
  // sensors is an ARRAY of sensor_info structs with a length of SENSOR_NUM
  // SENSOR_NUM is a macro in SensorController.h that equals the total number of
  // sensors on the car
  // SensorController.h has macros for each index that specify which sensor that
  // index belongs to
  // RIGHT_LINE_SENSOR = 0, LEFT_LINE_SENSOR = 1,
  // FRONT_IR_SENSOR = 2, SIDE_IR_SENSOR = 3
  sensor_info *sensors = Init_Sensors();

  // initialize the threads for each sensor
  // this is initialized as an array because SensorController.h has macros for
  // the indices
  // please refer to the comment above or SensorController.h for the indices
  pthread_t threads[SENSOR_NUM];

  // creates the LINE sensor threads and sends them the necessary structs
  if (pthread_create(&threads[RIGHT_LINE_SENSOR], NULL, Read_Sensor,
                     (void *)&sensors[RIGHT_LINE_SENSOR])) {
    printf("failed to create right line sensor thread\n");
    Free_Sensors(sensors);
    return 1;
  }

  if (pthread_create(&threads[LEFT_LINE_SENSOR], NULL, Read_Sensor,
                     (void *)&sensors[LEFT_LINE_SENSOR])) {
    printf("failed to create left line sensor thread\n");
    Free_Sensors(sensors);
    return 1;
  }

  // TODO: ADD 2 MORE THREADS FOR IR SENSORS
  // NOTE: YOU WILL NEED TO CHANGE SENSOR_NUM FROM 2 TO 4, THEN YOU SHOULD BE
  // ABLE TO JUST UNCOMMENT THIS CODE (IT IS NOT TESTED SO PLEASE VERIFY THAT IT
  // IS CORRECT)

  // creates the IR sensor threads and sends them the necessary structs
  //  if (pthread_create(&threads[FRONT_IR_SENSOR], NULL, Read_Sensor,
  //                     (void *)&sensors[FRONT_IR_SENSOR])) {
  //    printf("failed to create front IR sensor thread\n");
  //    Free_Sensors(sensors);
  //    return 1;
  //  }
  //
  //  if (pthread_create(&threads[SIDE_IR_SENSOR], NULL, Read_Sensor,
  //                     (void *)&sensors[SIDE_IR_SENSOR])) {
  //    printf("failed to create side IR sensor thread\n");
  //    Free_Sensors(sensors);
  //    return 1;
  //  }

  signal(SIGINT, sigintHandler);

  while (running) {
    // TODO: IMPLEMENT OBSTACLE AVOIDANCE HERE (IT SHOULD TAKE PRIORITY OVER
    // LINE DETECTION)

    // printf("Right sensor: %d\tLeft sensor: %d\n",
    // sensors[RIGHT_LINE_SENSOR].sensor_value,
    // sensors[LEFT_LINE_SENSOR].sensor_value);
    if (sensors[LEFT_LINE_SENSOR].sensor_value &&
        sensors[RIGHT_LINE_SENSOR].sensor_value) {
      printf("\n**************************************BOTH SENSORS "
             "TRIPPED**************************************\n");
    }

    while (sensors[LEFT_LINE_SENSOR].sensor_value &&
           !sensors[RIGHT_LINE_SENSOR].sensor_value) {
      Turn_Left(motors);
      usleep(500);
    }

    while (sensors[RIGHT_LINE_SENSOR].sensor_value &&
           !sensors[LEFT_LINE_SENSOR].sensor_value) {
      Turn_Right(motors);
      usleep(500);
    }

    Move_All_Forward(motors);
  }

  if (pthread_join(threads[RIGHT_LINE_SENSOR], NULL)) {
    printf("failed to join right line sensor\n");
    Free_Sensors(sensors);
    return 1;
  }

  if (pthread_join(threads[LEFT_LINE_SENSOR], NULL)) {
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
