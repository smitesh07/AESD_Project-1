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
#include <mqueue.h>
#include <errno.h>
#include <string.h>
#include "log.h"
#include "queue.h"

// To store queue attributes
struct mq_attr attr,old_attr;

// Message queue descriptors
mqd_t mqdes;

void initQueue(char *queueName) {
    // First we need to set up the attribute structure
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(QUEUE_t);
    attr.mq_flags = 0;

    char path[50];
    strcpy(path,queueName);
    // printf("Path %s", path);

    mq_unlink(queueName);
    
    // Open a queue with the attribute structure
    mqdes = mq_open (queueName, O_RDWR | O_CREAT, 0666, &attr);
    if (mqdes <0)
        perror("mq_open()");
}

void enQueueForLog(LOG_LEVEL level, char *msg, float value) {
    /* pointer to priority queue */
    QUEUE_t *prioQueue;
    prioQueue = (QUEUE_t *)malloc(sizeof(QUEUE_t));
    LOG_LEVEL prio;
    
    prioQueue->mtype = 1;
    (prioQueue->logQueue).level = level;
    strcpy((prioQueue->logQueue).msg, msg);
    (prioQueue->logQueue).value = value;
    prio = level;
       
    if (mq_send (mqdes, (const char *)prioQueue, sizeof(QUEUE_t), prio)< 0)
        perror ("mq_send()");

    free(prioQueue);   
}

void deQueueFromLog(void) {
     /* pointer to priority queue */
    QUEUE_t *prioQueue;
    prioQueue = (QUEUE_t *)malloc(sizeof(QUEUE_t));
    LOG_LEVEL prio;
    // printf("Current messages %d", attr.mq_curmsgs);
    // if (attr.mq_curmsgs > 0) {
        // printf("After");
        // There are some messages on this queue....eat em
        
        // First set the queue to not block any calls    
        attr.mq_flags = O_NONBLOCK;
        mq_setattr (mqdes, &attr, &old_attr);  
            
        // Now eat all of the messages
        while (mq_receive (mqdes, (char *)prioQueue, sizeof(QUEUE_t), &prio) != -1) {
            if((prioQueue->logQueue).level == ERROR)
                LOG_ERROR("%s",(prioQueue->logQueue).msg);
            else if ((prioQueue->logQueue).level == WARN)
                LOG_WARN("%s",(prioQueue->logQueue).msg); 
            else if ((prioQueue->logQueue).level == INFO)
                LOG_INFO("%s%f",(prioQueue->logQueue).msg, (prioQueue->logQueue).value);
            else if ((prioQueue->logQueue).level == DEBUG)
                LOG_DEBUG("%s",(prioQueue->logQueue).msg);
        }                
        
        fflush(filePtr);
        // The call failed.  Make sure errno is EAGAIN
        // if (errno != EAGAIN) 
        // { 
        //     perror ("mq_receive()");
        // _exit (EXIT_FAILURE);
        // }
        
        free(prioQueue);
        
        // Now restore the attributes
        mq_setattr (mqdes, &old_attr, 0);

        // mq_close (mqdes);         
    // }
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