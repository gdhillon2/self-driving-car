#include "main.h"
#include "lib/MotorController/MotorController.h"
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

int Create_Sensor_Threads(pthread_t thread[], void *sensor_struct) {
  // we only go to SENSOR_NUM - 2 because the last two sensor structs are not
  // threaded
  sensor_info *sensor_data = sensor_struct;
  for (int i = 0; i < SENSOR_NUM - 2; i++) {
    if (pthread_create(&thread[i], NULL, Read_Sensor, &sensor_data[i])) {
      return 1;
    }
  }
  return 0;
}

int Join_Sensor_Threads(pthread_t thread[]) {
  // we only go to SENSOR_NUM - 2 because the last two sensor structs are not
  // threaded
  for (int i = 0; i < SENSOR_NUM - 2; i++) {
    if (pthread_join(thread[i], NULL)) {
      return 1;
    }
  }
  return 0;
}

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
  // SENSOR_NUM is a macro in SensorController.h that equals the total number
  // of sensors on the car SensorController.h has macros for each index that
  // specify which sensor that index belongs to RIGHT_LINE_SENSOR = 0,
  // LEFT_LINE_SENSOR = 1, FRONT_IR_SENSOR = 2, SIDE_IR_SENSOR = 3
  sensor_info *sensors = Init_Sensors();

  // initialize the threads for each sensor
  // this is initialized as an array because SensorController.h has macros for
  // the indices
  // please refer to the comment above or SensorController.h for the indices
  // pthread_t threads[SENSOR_NUM];

  //  int thread_create_check = Create_Sensor_Threads(threads, sensors);
  //  if (thread_create_check) {
  //    printf("failed to create all threads\n");
  //    return 1;
  //  }

  signal(SIGINT, sigintHandler);

  int hard_left_turn = 0;
  int hard_right_turn = 0;
  while (running) {
    printf("Gets in loop \n");
    // double front_sonic_sensor =
    // Read_Sonic_Sensor(&sensors[FRONT_SONIC_SENSOR]); printf("Front Sonic
    // Runs\n"); printf("front sonic distance: %.1f\n", front_sonic_sensor);
    // double back_sonic_sensor =
    // Read_Sonic_Sensor(&sensors[BACK_SONIC_SENSOR]); printf("Back SOnic
    // Runs\n"); printf("back sonic distance: %.1f\n", back_sonic_sensor);
    //    if (front_sonic_sensor <= 10.0) {
    //      while (front_sonic_sensor <= 15.0 && running) {
    //        Shift_Left(motors);
    //        front_sonic_sensor =
    //        Read_Sonic_Sensor(&sensors[FRONT_SONIC_SENSOR]); printf("shifting
    //        left to avoid object\n");
    //      }
    //
    //      usleep(100000);
    //
    //      while (back_sonic_sensor >= 40.0) {
    //        Move_All_Forward(motors);
    //        back_sonic_sensor =
    //        Read_Sonic_Sensor(&sensors[BACK_SONIC_SENSOR]); printf("moving
    //        forward waiting to go past object\n");
    //      }
    //
    //      while (back_sonic_sensor < 40.0) {
    //        printf("%d\n", back_sonic_sensor);
    //        Move_All_Forward(motors);
    //        back_sonic_sensor =
    //        Read_Sonic_Sensor(&sensors[BACK_SONIC_SENSOR]); printf("object has
    //        been sensed, moving forward to go past object\n");
    //      }
    //
    //      while (back_sonic_sensor >= 40.0) {
    //        Shift_Right(motors);
    //        back_sonic_sensor =
    //        Read_Sonic_Sensor(&sensors[BACK_SONIC_SENSOR]); if
    //        (sensors[FRONT_LEFT_LINE_SENSOR].sensor_value) {
    //          break;
    //        }
    //      }
    //
    //      printf("going back to regular line detection\n");
    //    }

    // the general function of the line sensors is as follows:

    // as long as the front 2 sensors are on the line, the car will continue to
    // go forward no matter the back sensor inputs

    // if a back sensor comes in contact with the line, it will "queue" a turn
    // in the form of a flag so that the next time the front 2 sensors both fall
    // off the line, it will hard turn until both front sensors go back on the
    // line

    // if only one of the front sensors falls off the line (irrespective of any
    // turn flags) the car will soft turn back towards the line until both
    // sensors are back on the line

    // this if/else if block sets the turn flags and determines which direction
    // to turn when a hard turn is initiated
    // NOTE: this if/else if block does NOT initiate the turn, it ONLY sets what
    // the next turn WILL be, the turn timing is determined when the 2 front
    // sensors fall off the line in the following if/else if blocks
    if (gpioRead(BACK_LEFT_LINE_SENSOR_GPIO)) {
      printf("Hard Left ON!\n");
      hard_left_turn = 1;
      hard_right_turn = 0;
    } else if (gpioRead(BACK_RIGHT_LINE_SENSOR_GPIO)) {
      printf("Hard Right ON!\n");
      hard_left_turn = 0;
      hard_right_turn = 1;
    }

    // this if block determines a hard left turn, if the hard left turn flag
    // is active and both front line sensors are off the line, a hard left turn
    // is initiated until the front line sensors are back on the line, then the
    // flag is set back to 0 to indicate the turn is complete
    if (hard_left_turn && !gpioRead(FRONT_RIGHT_LINE_SENSOR_GPIO) &&
        !gpioRead(FRONT_LEFT_LINE_SENSOR_GPIO)) {

      // Turn_Left turns the car left until another MotorController function is
      // called
      // for this reason, we can call it once and simply spin in a while
      // loop while the 2 front sensors are not on a line once the two sensors
      // are back on the line, we can exit this entire if block
      Turn_Left(motors);

      // this while loop prints out the front sensor values while the car is
      // turning
      while ((!gpioRead(FRONT_RIGHT_LINE_SENSOR_GPIO) ||
             !gpioRead(FRONT_LEFT_LINE_SENSOR_GPIO)) && running) {
        printf("gpioRead FRONT LEFT: %d\tgpioRead FRONT RIGHT: %d\n",
               gpioRead(FRONT_LEFT_LINE_SENSOR_GPIO),
               gpioRead(FRONT_RIGHT_LINE_SENSOR_GPIO));
      }

      printf("Exiting hard left\n");

      hard_left_turn = 0;
    }
    // this else if block determines a hard right turn, if the hard right
    // turn flag is active and both front line sensors are off the line, a hard
    // right turn is initiated until the front line sensors are back on the
    // line, then the flag is set back to 0 to indicate the turn is complete
    else if (hard_right_turn && !gpioRead(FRONT_LEFT_LINE_SENSOR_GPIO) &&
             !gpioRead(FRONT_RIGHT_LINE_SENSOR_GPIO)) {

      // Turn_Right turns the car left until another MotorController function is
      // called
      // for this reason, we can call it once and simply spin in a while
      // loop while the 2 front sensors are not on a line once the two sensors
      // are back on the line, we can exit this entire if block
      Turn_Right(motors);

      // this while loop prints out the front sensor values while the car is
      // turning
      while ((!gpioRead(FRONT_LEFT_LINE_SENSOR_GPIO) ||
             !gpioRead(FRONT_RIGHT_LINE_SENSOR_GPIO)) && running) {
        printf("gpioRead FRONT LEFT: %d\ngpioRead FRONT RIGHT: %d\n",
               gpioRead(FRONT_LEFT_LINE_SENSOR_GPIO),
               gpioRead(FRONT_RIGHT_LINE_SENSOR_GPIO));
      }

      printf("Exiting hard right\n");

      hard_right_turn = 0;
    }
    // this else if block determines a soft left turn, if the front right
    // line sensor has fallen off the line but the front left one is still on
    // the line, it will do a soft turn to adjust back on the line as this is
    // considered a simple correction rather than a turn on the course
    else if (!gpioRead(FRONT_RIGHT_LINE_SENSOR_GPIO) &&
             gpioRead(FRONT_LEFT_LINE_SENSOR_GPIO)) {
      Soft_Turn_Left(motors);
    }
    // this else if block determines a soft right turn, if the front right
    // line sensor has fallen off the line but the front right one is still on
    // the line, it will do a soft turn to adjust back on the line as this is
    // considered a simple correction rather than a turn on the course
    else if (!gpioRead(FRONT_LEFT_LINE_SENSOR_GPIO) &&
             gpioRead(FRONT_RIGHT_LINE_SENSOR_GPIO)) {
      Soft_Turn_Right(motors);
    }
    // if both front line sensors read the line, move the car forward
    else if (gpioRead(FRONT_LEFT_LINE_SENSOR_GPIO) &&
             gpioRead(FRONT_RIGHT_LINE_SENSOR_GPIO)) {
      Move_All_Forward(motors);
    }
  }
  printf("exiting running loop\n");
  Stop_All_Motors(motors);

  //  int thread_join_check = Join_Sensor_Threads(threads);
  //  if (thread_join_check) {
  //    printf("failed to join threads\n");
  //    free(motors);
  //    Free_Sensors(sensors);
  //    return 1;
  //  }

  printf("\nshutting down successfully\n");
  free(motors);
  Free_Sensors(sensors);
  return 0;
}
