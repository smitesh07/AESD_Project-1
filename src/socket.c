/**
 * @file socket.c
 * @author Smitesh Modak and Ashish Tak
 * @brief : Implementation file for the Remote Socket Request handling task
 * @version 0.1
 * @date 2019-03-31
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "socket.h"


#define BUFFER_MAX_SIZE 100
#define QUERY_MAX_SIZE 10
#define DEFAULT_CONNECTION_PROTOCOL 0
#define NW_PORT_NUMBER 5000
#define MAX_QUEUED_CONNECTIONS 10


void *externSocketHandler (void *arg) {
    int sockDescriptor = 0, sockAccepted = 0, n=0;
    struct sockaddr_in serv_addr;
    luxUpdate * latestLux = (luxUpdate *)malloc(sizeof(luxUpdate));
    tempUpdate * latestTemp = (tempUpdate *)malloc(sizeof(tempUpdate));

    char txBuffer[BUFFER_MAX_SIZE], rxBuffer[QUERY_MAX_SIZE];

    //Create a socket inside the kernel (using IPv4 addresses - AF_INET)
    sockDescriptor = socket(AF_INET, SOCK_STREAM, DEFAULT_CONNECTION_PROTOCOL);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(NW_PORT_NUMBER);

    //Bind the structure to the created socket
    bind(sockDescriptor, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    //Make the socket a fully functional listening socket
    listen(sockDescriptor, MAX_QUEUED_CONNECTIONS);

    while (1) {
        //Wait until an incoming client request is made
        sockAccepted = accept(sockDescriptor, (struct sockaddr*)NULL, NULL);

        //The system call can be interrupted by a signal before a valid connection
        //arrived, and thus the value of sockAccepted can be -1
        while(sockAccepted>0) {
            //Once a valid connection is opened, keep reading the messages until the
            //client sends a termination (term) message
            n = read(sockAccepted, rxBuffer, sizeof(rxBuffer)-1);

            //Check the number of characters returned from the read() call 
            if (n>0) {
                rxBuffer[n] = '\0';
                printf("\nReceived: %s", rxBuffer);

                /*
                TODO: Depending on the command call the required APIs or return the last logging result
                */ 
                if (!strcmp(rxBuffer,"temp")) {
                    latestTemp= externReadTemp();
                    if (latestTemp->sensorConnected) {
                        sprintf(txBuffer, "Temperature: %f", latestTemp->temp);    
                    }
                    else {
                        sprintf(txBuffer, "Temperature sensor disconnected");
                    }
                }
                else if (!strcmp(rxBuffer,"lum")) {
                    latestLux = externReadLum();
                    if (latestLux->sensorConnected) {
                        sprintf(txBuffer, "Luminosity: %f. It is %s", latestLux->lux, (latestLux->brightnessState?"light":"dark"));    
                    }
                    else {
                        sprintf(txBuffer, "Luminous sensor disconnected");
                    }
                }
                else if (!strcmp(rxBuffer,"term")){
                    //Break on receiving the termination signal and proceed to close the file descriptor (i.e. socket) 
                    break;
                }
                else {
                    sprintf(txBuffer, "Invalid query. Try again");
                }
                write(sockAccepted, txBuffer, strlen(txBuffer));
            }
            //sleep(1);

        }

        //Close the current socket connection and listen for the next incoming connection
        close(sockAccepted);
    }

}