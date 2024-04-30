/****************************************************************
 * Class:: CSC-615-01 Spring 2024
 * Name:: Gurvir Dhillon
 * Student ID:: 923067350
 * Github-Name:: gdhillonSFSU
 * Project::
 * File:: GPIOHeader.h
 *
 * Description:: This is a header file for a personal library that
 * utilizes direct register access to manipulate the GPIO pins.
 * **************************************************************/
#ifndef GPIO_HEADER
#define GPIO_HEADER

// gpio functions
void setup_io();

void set_gpio_output(int pin);
void set_gpio_input(int pin);

void set_pin_high(int pin);
void set_pin_low(int pin);

int get_pin_value(int pin);

#endif // GPIO_HEADER
