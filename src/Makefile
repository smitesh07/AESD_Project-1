#  File name: Makefile
#  Description: Makefile
#  Author: Smitesh Modak
#  Date: 02/27/2019

include sources.mk

CC = gcc
CPPFLAGS += $(addprefix -I, $(INCLUDE_H))
ifeq ($(PLATFORM),HOST)
	CC = gcc
endif

ifeq ($(PLATFORM),BBG)
	CC = arm-linux-gcc
endif

SRCS = main.c \
       log.c  \
	   timer.c \
	   tempSensor.c

TARGET = test

OBJS = $(SRCS:.c=.o)

%.o : %.c 
	$(CC) $(CPPFLAGS) -c $^ -o  $@

.PHONY: all clean

all: $(TARGET)
	#scp -r ../BBG root@10.0.0.69:/Smith/

$(TARGET): $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o $@ -lpthread -lrt

clean:
	rm -f $(OBJS) $(TARGET)
