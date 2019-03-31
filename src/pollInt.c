/**
 * @file pollInt.c
 * @author Smitesh Modak and Ashish Tak
 * @brief  source file to initialize polling mechanism
 * @version 0.1
 * @date 2019-03-27
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include "SimpleGPIO.h"
#include "pollInt.h"

void pollInit(unsigned int gpio, poll_t *pollFds)
{
    int ret;
    int n;

    char buf[MAX_BUF];
    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);
    pollFds->f = open(buf, O_RDONLY);
    if (pollFds->f == -1) {
        perror("gpio/fd_open");
    }
    
    n = read(pollFds->f, &(pollFds->value), sizeof(pollFds->value));
    if (n > 0) {
        printf("Initial value=%c\n", pollFds->value[0]);
        lseek(pollFds->f, 0, SEEK_SET);
    }

    pollFds->poll_fds[0].fd = pollFds->f;
    pollFds->poll_fds[0].events = POLLPRI | POLLERR;
}