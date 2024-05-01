#ifndef SENSOR_CONTROLLER_H
#define SENSOR_CONTROLLER_H

#include "../../main.h"

#define RIGHT_LINE_SENSOR 0
#define LEFT_LINE_SENSOR 1
#define FRONT_IR_SENSOR 2
#define SIDE_IR_SENSOR 3

// struct to be passed to the thread that contains
// gpio pin and sensor value
typedef struct {
  int gpio_pin;
  int sensor_value;
} sensor_info;

sensor_info *Init_Sensors();
void Free_Sensors(sensor_info *sensor_array);
void *Read_Sensors(void *args);
void Running_Test();
#endif // SENSOR_CONTROLLER_H
