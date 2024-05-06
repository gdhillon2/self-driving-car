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
  gpioSetMode(FSONIC_SENSOR_ECHO, PI_INPUT);
  gpioSetMode(FSONIC_SENSOR_TRIG, PI_OUTPUT);

  // initialize the structs used for the 2 sensors
  // TODO: NEED TO INITIALIZE THE TWO IR STRUCTS AS WELL (JUST UNCOMMENT THE
  // INITIALIZATIONS)
  sensor_info *sensor_array = malloc(sizeof(sensor_info) * SENSOR_NUM);

  sensor_array[RIGHT_LINE_SENSOR].gpio_pin = RIGHT_LINE_SENSOR_GPIO;
  sensor_array[RIGHT_LINE_SENSOR].sensor_value = 0;
  sensor_array[RIGHT_LINE_SENSOR].distance = -1;

  sensor_array[LEFT_LINE_SENSOR].gpio_pin = LEFT_LINE_SENSOR_GPIO;
  sensor_array[LEFT_LINE_SENSOR].sensor_value = 0;
  sensor_array[LEFT_LINE_SENSOR].distance = -1;

  sensor_array[FRONT_SONIC_SENSOR].gpio_pin = FSONIC_SENSOR_TRIG;
  sensor_array[FRONT_SONIC_SENSOR].sensor_value = 0;
  sensor_array[FRONT_SONIC_SENSOR].distance = 1000;

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

double Read_Sonic_Sensor(sensor_info *sensor) {

  int speed_of_sound = 34000;   // cm/s
  int trig_sleep_duration = 10; // microseconds
  double start, stop, distance;

  // set trig pin to high
  gpioWrite(TRIG_PIN, PI_HIGH);

  // sleep for 10 microseconds
  usleep(trig_sleep_duration);

  // set trig pin to low
  gpioWrite(TRIG_PIN, PI_LOW);

  // while echo pin is low do nothing
  while (gpioRead(ECHO_PIN) == PI_LOW) {
  }

  // once its high, set start time
  start = time_time();

  // wait for echo pin to go low
  while (gpioRead(ECHO_PIN) == PI_HIGH) {
  }

  // when echo pin goes low, end time
  stop = time_time();

  // do calculations s = (vt)/2
  distance = (speed_of_sound(stop - start)) / 2;

  // print value and keep going
  printf("distance: %.1f cm\n", distance);

  return distance
}

// int Test_Sonic_Sensor(sensor_info sensor) {
//
// }
