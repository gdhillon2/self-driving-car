/****************************************************************
 * Class:: CSC-615-01 Spring 2024
 * Name:: Gurvir Dhillon
 * Student ID:: 923067350
 * Github-Name:: gdhillonSFSU
 * Project::
 * File:: SensorController.c
 *
 * Description::
 * **************************************************************/
#include "SensorController.h"
#include <pigpio.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define RIGHT_LINE_SENSOR_GPIO 17
#define LEFT_LINE_SENSOR_GPIO 27
#define FSONIC_SENSOR_TRIG 21
#define FSONIC_SENSOR_ECHO 20
#define BSONIC_SENSOR_TRIG -1 // TODO: UPDATE VALUE WITH ACTUAL GPIO PIN
#define BSONIC_SENSOR_ECHO -1 // TODO: UPDATE VALUE WITH ACTUAL GPIO PIN

// initializes the gpio pins needed
// initializes the sensor structs required
/***************************************************
 * CALLER IS RESPONSIBLE FOR FREEING THE RETURNED PTR
 ***************************************************/
sensor_info *Init_Sensors() {
  gpioInitialise();
  gpioSetMode(RIGHT_LINE_SENSOR_GPIO, PI_INPUT);
  gpioSetMode(LEFT_LINE_SENSOR_GPIO, PI_INPUT);

  // initialize the structs used for the 2 sensors
  // TODO: NEED TO INITIALIZE THE TWO IR STRUCTS AS WELL (JUST UNCOMMENT THE
  // INITIALIZATIONS)
  sensor_info *sensor_array = malloc(sizeof(sensor_info) * SENSOR_NUM);

  sensor_array[RIGHT_LINE_SENSOR].gpio_pin = RIGHT_LINE_SENSOR_GPIO;
  sensor_array[RIGHT_LINE_SENSOR].sensor_value = 0;

  sensor_array[LEFT_LINE_SENSOR].gpio_pin = LEFT_LINE_SENSOR_GPIO;
  sensor_array[LEFT_LINE_SENSOR].sensor_value = 0;

  //  sensor_array[FRONT_IR_SENSOR].gpio_pin = FRONT_IR_SENSOR_GPIO;
  //  sensor_array[FRONT_IR_SENSOR].sensor_value = 0;
  //
  //  sensor_array[SIDE_IR_SENSOR].gpio_pin = SIDE_IR_SENSOR_GPIO;
  //  sensor_array[SIDE_IR_SENSOR].sensor_value = 0;

  return sensor_array;
}

void Free_Sensors(sensor_info *sensor_array) {
  gpioTerminate();
  free(sensor_array);
}

// the threaded function: reads the gpio pin of the corresponding sensor
// and writes value to gpio_pin member
void *Read_Sensor(void *arg) {
  sensor_info *sensor_args = arg;
  while (running) {
    sensor_args->sensor_value = gpioRead(sensor_args->gpio_pin);
  }
  pthread_exit(NULL);
}

int Test_Sonic_Sensor(sensor_info sensor) {}
