#ifndef SENSOR_CONTROLLER_H
#define SENSOR_CONTROLLER_H

#include "../../main.h"

// use these as indexes for accessing a specific sensor struct
// in sensor_array
#define RIGHT_LINE_SENSOR 0
#define LEFT_LINE_SENSOR 1
#define FRONT_SONIC_SENSOR 2
#define SIDE_SONIC_SENSOR 3

#define TEST_IR_GPIO 22
#define SENSOR_NUM                                                             \
  4 // TODO: CHANGE THIS TO 4 WHEN SONIC SENSORS ARE IMPLEMENTED

// struct to be passed to the thread that contains
// gpio pin and sensor value
typedef struct {
  int gpio_pin;
  int sensor_value;
  double distance;
} sensor_info;

sensor_info *Init_Sensors();
void Free_Sensors(sensor_info *sensor_array);
void *Read_Sensor(void *args);
double Read_Sonic_Sensor(sensor_info *sensor);
#endif // SENSOR_CONTROLLER_H
