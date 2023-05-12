#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<arpa/inet.h>
#include<sys/socket.h>

void PrintErrorAndExit(char *lanErrorString)
{
    perror(lanErrorString);
    exit(1);
}

int main(int argc,char** args)
{
    // do nothing waiting to be terminated
	argc=argc;
    int count=0;
    int lnPid = getpid();

	char name[64];
	strcpy(name,args[0]);
	char ip[64];
	strcpy(ip,args[1]);
	char port[64];
	strcpy(port,args[2]);

    //===========================================
	
	struct sockaddr_in lsSAMe, lsSAOther;

    int lnSocketId = 0; // contains socket handle
    int lnReceiveLen = 0;  // number of bytes received on socket
	lnReceiveLen=lnReceiveLen;
    socklen_t lnSockStructLen = sizeof(lsSAOther); // length of socket structure

    // clear receive buffer
    char lanRecBuf[256] = {0};


    //create a UDP socket
    if ((lnSocketId=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        PrintErrorAndExit("socket");
    }

    // zero out the structure
    memset((char *) &lsSAMe, 0, sizeof(lsSAMe));
    lsSAMe.sin_family = AF_INET; // use IPV4 network addressing
    lsSAMe.sin_port = htons(atoi(port)); // convert to network byte order
    lsSAMe.sin_addr.s_addr = inet_addr(ip);


    //bind socket to our specific port
    if( bind(lnSocketId, (struct sockaddr*)&lsSAMe, sizeof(lsSAMe) ) == -1)
    {
        PrintErrorAndExit("bind");
    }
	printf("\n");
	printf("%d, %s : Inside child with PID %d, Address %s port:%d\n",++count,name,lnPid,ip,atoi(port));
    //keep listening for data
	printf("New Child borned...");
	printf("\n");
    while(1)
    {
   
        fflush(stdout);

        //try to receive some data, this is a blocking call
        // -1 indicates an error. anything 0 or above is assumed to be the number of bytes received.
        if ((lnReceiveLen = recvfrom(lnSocketId, lanRecBuf, 256, 0,
            (struct sockaddr *) &lsSAOther, &lnSockStructLen)) == -1)
        {
            PrintErrorAndExit("recvfrom()");
        }

        //print details of the client/peer and the data received
		printf("\n");
        printf("Received data from %s:%d\n", inet_ntoa(lsSAOther.sin_addr), ntohs(lsSAOther.sin_port));
        printf("Data: %s\n" , lanRecBuf);
		if(strcmp(lanRecBuf,"kill")==0){
			exit(-1);
		}
    }

    // close the socket resource
    close(lnSocketId);
	
    return 0;
}
