/**
 * @file remoteClient.c
 * @author Smitesh Modak and Ashish Tak
 * @brief File to be run from an external client to query the server running on the BBG
 * @version 0.1
 * @date 2019-03-25
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 


#define BUFFER_MAX_SIZE 100
#define QUERY_MAX_SIZE 10
#define DEFAULT_CONNECTION_PROTOCOL 0
#define NW_PORT_NUMBER 5000


int main(int argc, char *argv[])
{
    int sockDescriptor = 0, n = 0;
    char rxBuffer[BUFFER_MAX_SIZE], txBuffer[QUERY_MAX_SIZE];
    struct sockaddr_in serv_addr; 

    if(argc != 2)
    {
        printf("\nPlease input ip of the server to be connected too!\n");
        return 1;
    } 

    //Create a socket (using IPv4 addresses - AF_INET)
    if((sockDescriptor = socket(AF_INET, SOCK_STREAM, DEFAULT_CONNECTION_PROTOCOL)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(NW_PORT_NUMBER); 

    //Convert the ip address from the command line string to integer form
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    //Connect this socket with the socket of the remote host
    if( connect(sockDescriptor, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Failed to connect to the remote socket.\n");
       return 1;
    }

    while (1) {
	    printf("\n\nEnter the message to be queried: \
        \nTemperature: 'temp'\nLuminosity: 'lum'\nTerminate connection: 'term'\n");
	    scanf("%s", txBuffer);

        //Send the query message to the remote socket and wait for the response
	    write(sockDescriptor, txBuffer, strlen(txBuffer));
	    n = read(sockDescriptor, rxBuffer, sizeof(rxBuffer)-1);
	    if (n>0) {
		    rxBuffer[n] = '\0';
		    printf("\nReceived: %s\n", rxBuffer);
	    }
	    else if (n==0){
		    printf("\nTerminating remote socket connection.. \n");
		    break;
	    }
	    else {
		    printf("\nRead error.. ");
	    }
    }

    return 0;
}