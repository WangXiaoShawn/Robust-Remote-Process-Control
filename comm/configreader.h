#include <stdlib.h>

#define MAXLINE_LEN 1024
#define MAX_NUM_CHILDREN 3


typedef struct sysconfig
{
    char lanServerAddress[256];
    char lanChild1Proc[256];
	int PID1;
    char lnChild1Port[256];
    char lanChild2Proc[256];
	int PID2;
    char lnChild2Port[256];
    char lanChild3Proc[256];
	int PID3;
    char lnChild3Port[256];
	
    
} SystemConfig;

int GetSystemConfig(SystemConfig * sysConfig);