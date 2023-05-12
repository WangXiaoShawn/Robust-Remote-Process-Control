/*
    Author: Joe Waclawski
    Description: Simple udp client adapted from material found on the internet
    This is the "Client" side of a UDP communication example. It will prompt
    the user for a message, and send it to the server. It will then  display
    the server's response.
*/

#include<stdio.h>    //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h> //close
#include "../comm/configreader.h"

#define BUFLEN 512    //Max length of buffer

// simple routine to print an error and exit
void PrintErrorAndExit(char *lanErrorString)
{
    perror(lanErrorString);
    exit(1);
}

int main(void)
{
	int lnSocketId = 0;
	while(1){
		SystemConfig syscon;
		GetSystemConfig(&syscon);
		// get the socket setup information
		
		int port=0;
		printf("Enter Port: ");
		scanf("%d",&port);
	
		// socket structure to define properties
		struct sockaddr_in lsSAOther;
	
		// place to store socket handle
		socklen_t lnSockStructLen=sizeof(lsSAOther);
	
		// attempt to create a socket
		if ( (lnSocketId=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		{
			PrintErrorAndExit("socket");
		}
	// zero out the structure used to define where the
		// data is going (destination) on the socket
		memset((char *) &lsSAOther, 0, sizeof(lsSAOther));
	
		// initialize the destination properties
		lsSAOther.sin_family = AF_INET;  // IPV4
		lsSAOther.sin_port = htons(port); // convert port to network byte order
		lsSAOther.sin_addr.s_addr = inet_addr(syscon.lanServerAddress);
	
		// set the destination address in the destination properties
		if (inet_aton(syscon.lanServerAddress, &lsSAOther.sin_addr) == 0)
		{
			PrintErrorAndExit("inet_aton() failed\n");
		}
		
		
		char message[]="kill";
        //send the Message
        if (sendto(lnSocketId, message, strlen(message)+1 , 0 ,
            (struct sockaddr *) &lsSAOther, lnSockStructLen)==-1)
        {
            PrintErrorAndExit("sendto()");
        }
    }

    // close our socket resource
    close(lnSocketId);
return 0;
}
