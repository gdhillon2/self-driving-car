# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -D USE_BCM2835_LIB

# Libraries
LIBS = -lm -lbcm2835

# Source files
SRCS = assignment3.c DEV_Config.c PCA9685.c gpioheader.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = assignment3

# Default target
all: $(EXEC)

# Rule to build the executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(EXEC)

# Rule to compile C source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(EXEC)

