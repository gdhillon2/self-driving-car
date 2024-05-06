#ifndef MAIN_H
#define MAIN_H

#include <signal.h>

// this flag is used to stop the motors and the threads
// then terminate the entire program
extern volatile sig_atomic_t running;
#endif // MAIN_H
