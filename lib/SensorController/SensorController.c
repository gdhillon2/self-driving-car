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
#include "../GPIOHeader/GPIOHeader.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define RIGHT_SENSOR_GPIO 17
#define LEFT_SENSOR_GPIO 27

// initializes the gpio pins needed
// initializes the sensor structs required
/***************************************************
 * CALLER IS RESPONSIBLE FOR FREEING THE RETURNED PTR
 ***************************************************/

sensor_info *Init_Sensors() {
  setup_io();
  set_gpio_input(RIGHT_SENSOR_GPIO);
  set_gpio_input(LEFT_SENSOR_GPIO);

  // initialize the structs used for the 2 sensors
  sensor_info *sensor_array = malloc(sizeof(sensor_info) * 2);

  sensor_array[RIGHT_SENSOR].gpio_pin = RIGHT_SENSOR_GPIO;
  sensor_array[RIGHT_SENSOR].sensor_value = -1;

  sensor_array[LEFT_SENSOR].gpio_pin = LEFT_SENSOR_GPIO;
  sensor_array[LEFT_SENSOR].sensor_value = -1;

  return sensor_array;
}

void Free_Sensors(sensor_info *sensor_array) { free(sensor_array); }

// the threaded function: reads the gpio pin of the corresponding sensor
// and writes value to gpio_pin member
void *Read_Sensor(void *arg) {
  sensor_info *sensor_args = arg;
  sensor_args->sensor_value = get_pin_value(sensor_args->gpio_pin);
  return NULL;
}

void Running_Test() {
  while (running) {
    printf("running\n");
    sleep(1);
  }
}
