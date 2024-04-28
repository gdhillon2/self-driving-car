/****************************************************************
 * Class:: CSC-615-01 Spring 2024
 * Name:: Gurvir Dhillon
 * Student ID:: 923067350
 * Github-Name:: gdhillon2
 * Project:: Assignment 1 - Traffic Light
 * File:: gpioheader.c
 *
 * Description:: This is the source file for a personal library that
 * utilizes direct register access to manipulate the GPIO pins.
 * **************************************************************/
#define BCM2708_PERI_BASE 0x3F000000
#define GPIO_BASE (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "gpioheader.h"

#define PAGE_SIZE (4 * 1024)
#define BLOCK_SIZE (4 * 1024)

int mem_fd;
void *gpio_map;

// I/O access
volatile unsigned *gpio;

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio + ((g) / 10)) &= ~(7 << (((g) % 10) * 3))
#define OUT_GPIO(g) *(gpio + ((g) / 10)) |= (1 << (((g) % 10) * 3))
#define SET_GPIO_ALT(g, a) *(gpio + (((g) / 10))) |= (((a) <= 3 ? (a) + 4 : (a) == 4 ? 3  \
                                                                                     : 2) \
                                                      << (((g) % 10) * 3))

#define GPIO_SET *(gpio + 7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio + 10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio + 13) & (1 << g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio + 37)     // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio + 38) // Pull up/pull down clock

// maps gpio registers to virtual memory space
void setup_io()
{
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0)
   {
      printf("can't open /dev/mem \n");
      exit(-1);
   }

   /* mmap GPIO */
   gpio_map = mmap(
       NULL,                   // Any adddress in our space will do
       BLOCK_SIZE,             // Map length
       PROT_READ | PROT_WRITE, // Enable reading & writting to mapped memory
       MAP_SHARED,             // Shared with other processes
       mem_fd,                 // File to map
       GPIO_BASE               // Offset to GPIO peripheral
   );

   close(mem_fd); // No need to keep mem_fd open after mmap

   if (gpio_map == MAP_FAILED)
   {
      printf("mmap error %d\n", (int)gpio_map); // errno also set!
      exit(-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;
}

// sets gpio pin as an output pin
void set_gpio_output(int pin)
{
   INP_GPIO(pin);
   OUT_GPIO(pin);
   printf("setting pin %d to output\n", pin);
   return;
}

// sets gpio pin as an input pin
void set_gpio_input(int pin)
{
   INP_GPIO(pin);
   printf("setting pin %d to input\n", pin);
   return;
}

// sets voltage going to pin as high
void set_pin_high(int pin)
{
   GPIO_SET = 1 << pin;
   printf("setting pin %d to high\n", pin);
   return;
}

// sets voltage goign to pin as low
void set_pin_low(int pin)
{
   GPIO_CLR = 1 << pin;
   printf("setting pin %d to low\n", pin);
   return;
}

// returns 1 for high voltage and 0 for low voltage on pin
int get_pin_value(int pin)
{
   return GET_GPIO(pin);
}
