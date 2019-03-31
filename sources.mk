#Defining common source files to be used
SRCS = src/main.c \
       src/log.c  \
	   src/timer.c \
	   src/tempSensor.c\
       src/queue.c \
	   src/lumSensor.c \
	   src/i2c.c \
	   src/tmp102.c \
	   src/SimpleGPIO.c \
	   src/pollInt.c \
	   src/socket.c \
	   src/apds9301.c

#Defining paths for the header files
INCLUDE_H = includes/