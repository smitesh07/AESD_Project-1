/**
 * @file queue.c
 * @author Smitesh Modak and Ashish Tak
 * @brief queue source file that initializes queue, enqueues and dequeues message queue
 * @version 0.1
 * @date 2019-03-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <syscall.h>
#include <string.h>
#include "log.h"
#include "queue.h"

/* pointer to priority queue */
QUEUE_t *prioQueue;

void initQueue(int msqid, const char *queueName) {
    prioQueue = (QUEUE_t *)malloc(sizeof(QUEUE_t));
    key_t key;
    const char path[50];
    strcpy(path,queueName);
    // ftok to generate unique key 
    key = ftok(*path, 65); 
    // msgget creates a message queue 
    // and returns identifier 
    msqid = msgget(key, 0666 | IPC_CREAT); 
}


void enQueueForLog(int msqid, LOG_LEVEL level, char *msg, int value) {
    prioQueue->mtype = 1;
    (prioQueue->logQueue).level = level;
    strcpy((prioQueue->logQueue).msg, msg);
    (prioQueue->logQueue).value = value;
    // msgsnd to send message 
    msgsnd(msqid, prioQueue, sizeof(QUEUE_t), 0);    
}

void deQueueFromLog(int msqid) {
    // msgrcv to receive message 
    msgrcv(msqid, prioQueue, sizeof(QUEUE_t), 1, 0);
    LOG_INFO("%s %d",(prioQueue->logQueue).msg, (prioQueue->logQueue).value);
    // if (logQueue->level == ERROR)
    //     {
    //     LOG_ERROR("Error writing successful");
    //     }
    // else if (logQueue->level == INFO)
    //     {
    //     LOG_INFO("Info writing successful %d",logQueue->value);
    //     }
    // else if (logQueue->level == WARN)
    //     {
    //     LOG_WARN("Warn writing successful");
    //     }
}
