#ifndef SENSOR_CONTROLLER_H
#define SENSOR_CONTROLLER_H

#include "../../main.h"

// use these as indexes for accessing a specific sensor struct
// in sensor_array
#define BACK_RIGHT_LINE_SENSOR 0
#define FRONT_RIGHT_LINE_SENSOR 1
#define FRONT_LEFT_LINE_SENSOR 2
#define BACK_LEFT_LINE_SENSOR 3
#define FRONT_SONIC_SENSOR 4
#define BACK_SONIC_SENSOR 5

#define BACK_LEFT_LINE_SENSOR_GPIO 5
#define FRONT_LEFT_LINE_SENSOR_GPIO 6
#define FRONT_RIGHT_LINE_SENSOR_GPIO 27
#define BACK_RIGHT_LINE_SENSOR_GPIO 17
#define FSONIC_SENSOR_TRIG 21
#define FSONIC_SENSOR_ECHO 20
#define BSONIC_SENSOR_TRIG 23
#define BSONIC_SENSOR_ECHO 24

#define SENSOR_NUM 6

// struct to be passed to the thread that contains
// gpio pin and sensor value
typedef struct {
  int gpio_pin;
  int gpio_pin_2;
  int sensor_value;
  double distance;
} sensor_info;

sensor_info *Init_Sensors();
void Free_Sensors(sensor_info *sensor_array);
void *Read_Sensor(void *args);
double Read_Sonic_Sensor(sensor_info *sensor);
#endif // SENSOR_CONTROLLER_H
