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
  sleep(1);
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

  signal(SIGINT, sigintHandler);

  while (running) {
    double front_sonic_sensor = Read_Sonic_Sensor(&sensors[FRONT_SONIC_SENSOR]);
    printf("front sonic distance: %.1f\n", front_sonic_sensor);
    // double side_sonic_sensor =
    // Read_Sonic_Sensor(&sensors[SIDE_SONIC_SENSOR]);
    double back_sonic_sensor = Read_Sonic_Sensor(&sensors[BACK_SONIC_SENSOR]);
    printf("back sonic distance: %.1f\n", back_sonic_sensor);
    if (front_sonic_sensor <= 10.0) {
      //    running = 0;
      //    Shift_Left(motors);
      //    sleep(1);
      //    Move_All_Forward(motors);
      //    sleep(1);
      //    Shift_Right(motors);
      //    sleep(1);
      while (front_sonic_sensor <= 15.0 && running) {
        Shift_Left(motors);
        front_sonic_sensor = Read_Sonic_Sensor(&sensors[FRONT_SONIC_SENSOR]);
        printf("shifting left to avoid object\n");
      }

      usleep(1300000);

      while (back_sonic_sensor >= 50.0) {
        // while(!sensors[SIDE_SONIC_SENSOR].sensor_value)
        Move_All_Forward(motors);
        back_sonic_sensor = Read_Sonic_Sensor(&sensors[BACK_SONIC_SENSOR]);
        printf("moving forward waiting to go past object\n");
      }

      while (back_sonic_sensor < 50.0) {
        printf("%d\n", back_sonic_sensor);
        Move_All_Forward(motors);
        back_sonic_sensor = Read_Sonic_Sensor(&sensors[BACK_SONIC_SENSOR]);
        printf("object has been sensed, moving forward to go past object\n");
      }

      usleep(70000);

      while (back_sonic_sensor >= 50.0) {
        Shift_Right(motors);
        back_sonic_sensor = Read_Sonic_Sensor(&sensors[BACK_SONIC_SENSOR]);
        if (sensors[LEFT_LINE_SENSOR].sensor_value) {
          break;
        }
      }

      printf("going back to regular line detection\n");
    }
    //    if (sensors[RIGHT_LINE_SENSOR].sensor_value &&
    //    sensors[LEFT_LINE_SENSOR].sensor_value) {
    //      Move_All_Forward(motors);
    //    }
    else if (!sensors[RIGHT_LINE_SENSOR].sensor_value &&
             !sensors[LEFT_LINE_SENSOR].sensor_value) {
      Turn_Left(motors);
    } else if (sensors[RIGHT_LINE_SENSOR].sensor_value) {
      Turn_Right(motors);
    }
    // else if (sensors[LEFT_LINE_SENSOR].sensor_value) {
    else {
      Move_All_Forward(motors);
    }
  }
  Stop_All_Motors(motors);

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
  printf("\nshutting down\n");
  free(motors);
  Free_Sensors(sensors);
  return 0;
}
