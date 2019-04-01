#  File name: Makefile
#  Description: Makefile
#  Author: Smitesh Modak and Ashish Tak
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

TARGET = project1

OBJS = $(SRCS:.c=.o)

%.o : %.c 
	$(CC) $(CPPFLAGS) -c $^ -o  $@

.PHONY: all clean

all: $(TARGET)
ifeq ($(PLATFORM),BBG)
	scp -r project1 root@10.0.0.67:/Project1/Test/
endif

$(TARGET): $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o $@ -lpthread -lrt -lm

clean:
	rm -f $(OBJS) $(TARGET)
