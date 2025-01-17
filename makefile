DIR_OBJ = ./lib
DIR_BIN = ./bin
DIR_Config = ./lib/Config
DIR_PCA9685 = ./lib/PCA9685
DIR_MotorController = ./lib/MotorController
DIR_MotorTest = ./lib/MotorTest
DIR_GPIOHeader = ./lib/GPIOHeader
DIR_SensorController = ./lib/SensorController

OBJ_C = $(wildcard ${DIR_OBJ}/*.c ${DIR_Config}/*.c ${DIR_PCA9685}/*.c ${DIR_MotorController}/*.c ${DIR_MotorTest}/*.c ${DIR_GPIOHeader}/*.c ${DIR_SensorController}/*.c)
OBJ_O = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${OBJ_C}))

TARGET = main
#BIN_TARGET = ${DIR_BIN}/${TARGET}

CC = gcc

DEBUG = -g -O0 -Wall
CFLAGS += $(DEBUG)

# USELIB = USE_BCM2835_LIB
# USELIB = USE_WIRINGPI_LIB
USELIB = USE_DEV_LIB
DEBUG = -D $(USELIB) 
LIB = -lm -lpigpio

${TARGET}:${OBJ_O} ${DIR_BIN}/${TARGET}.o
	$(CC) $(CFLAGS) $(OBJ_O) ${DIR_BIN}/${TARGET}.o -o $@ $(LIB)

${DIR_BIN}/%.o : %.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB) -I $(DIR_OBJ) -I $(DIR_Config) -I $(DIR_PCA9685)

${DIR_BIN}/%.o : $(DIR_OBJ)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB) -I $(DIR_Config)
    
${DIR_BIN}/%.o : $(DIR_Config)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB)

${DIR_BIN}/%.o : $(DIR_PCA9685)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB) -I $(DIR_Config)

${DIR_BIN}/%.o : $(DIR_MotorController)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB) -I $(DIR_Config) -I $(DIR_PCA9685)

${DIR_BIN}/%.o : $(DIR_MotorTest)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB) -I $(DIR_Config) -I $(DIR_PCA9685)

${DIR_BIN}/%.o : $(DIR_GPIOHeader)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB) -I $(DIR_Config) -I $(DIR_PCA9685)

${DIR_BIN}/%.o : $(DIR_SensorController)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB) -I $(DIR_Config) -I $(DIR_PCA9685)

clean :
	rm $(DIR_BIN)/*.o
	rm $(TARGET)
