#ifndef SENSOR_CONTROLLER_H
#define SENSOR_CONTROLLER_H

#include "../../main.h"

#define RIGHT_SENSOR 0
#define LEFT_SENSOR 1

// struct to be passed to the thread that contains
// gpio pin and name of sensor
typedef struct {
  int gpio_pin;
  int sensor_value;
} sensor_info;

sensor_info *Init_Sensors();
void Free_Sensors();
void *Read_Sensors(void *args);
void Running_Test();
#endif // SENSOR_CONTROLLER_H
