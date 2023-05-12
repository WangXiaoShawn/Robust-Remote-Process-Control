/*
    Author: Joe Waclawski
    Description: Reads a socket setup file in the format provided below.
    Can read the environment variable CONFIGDIR to get the directory
    that contains the configuration file
    
    # as first character denotes a comment line
    
    server_address:<dotted IPV4 address>
    server_port:<port>
*/
#include <stdio.h>
#include <string.h>
#include "configreader.h"

#define CONFIGFILE "sysconfig.ini"

int GetSystemConfig(SystemConfig * sysConfig)
{
    char lanInputLine[MAXLINE_LEN];
    const char lanToken[2] = ":";
    char * lpTag;
    char * lpValue;
    
    FILE * lpFile = NULL;
    
    // see if the environment variable is set with the path to our configuration directory
    char * ConfigDir = getenv("SYSCONFIGDIR");
	char ConfigFile[1024] = {0};
    if (ConfigDir != NULL)
    {
        // include the CONFIGDIR as specified by the enironment variable
        strncpy(ConfigFile,ConfigDir,sizeof(ConfigFile) - strlen(ConfigFile));
        strncat(ConfigFile,"/",sizeof(ConfigFile) - strlen(ConfigFile));
    }
    else
        return -1;
	/*
	char * ConfigDir = getenv("SYSCONFIGDIR");
    char ConfigFile[1024] = {0};
    if (ConfigDir != NULL)
    {
        // include the CONFIGDIR as specified by the enironment variable
        strncpy(ConfigFile,ConfigDir,sizeof(ConfigFile) - strlen(ConfigFile));
        strncat(ConfigFile,"/",sizeof(ConfigFile) - strlen(ConfigFile));
    }
    else
        return -1;
    */
	
    // append the filename that includes the configuratoin. 
    // if CONFIGDIR is not specified by the CONFIGDIR environment variable, then this 
    // file is expected to be in the directory where the program is run
    // strncat(ConfigFile,CONFIGFILE,sizeof(ConfigFile) - strlen(ConfigFile));
    
    // print the full file name for debug
    strncat(ConfigFile,CONFIGFILE,sizeof(ConfigFile) - strlen(ConfigFile));
    printf("Config file = %s\n",ConfigFile);
    lpFile = fopen(ConfigFile,"r");
    if (lpFile != NULL)
    {
        while (fgets(lanInputLine,MAXLINE_LEN,lpFile) != NULL)
        {
            if (lanInputLine[0] == '#') continue;
            // we have to assume the input file is in the correct format
            // get first token (the parameter)
            lpTag = strtok(lanInputLine,lanToken);
            if (strcmp(lpTag,"server_address") == 0)
            {
                lpValue = strtok(NULL,lanToken);
                strcpy(sysConfig->lanServerAddress,lpValue);
		//
		if(sysConfig->lanServerAddress[strlen(sysConfig->lanServerAddress)-1]=='\r')
			sysConfig->lanServerAddress[strlen(sysConfig->lanServerAddress)-1]='\0';

            }
            else if (strcmp(lpTag,"proc1") == 0)
            {
                // program name
                lpValue = strtok(NULL,lanToken);
                strcpy(sysConfig->lanChild1Proc,lpValue);
                
                // get port
                lpValue = strtok(NULL,lanToken);
                strcpy(sysConfig->lnChild1Port,lpValue);
		if(sysConfig->lnChild1Port[strlen(sysConfig->lnChild1Port)-1]=='\r')
			sysConfig->lnChild1Port[strlen(sysConfig->lnChild1Port)-1]='\0';
            }
            else if (strcmp(lpTag,"proc2") == 0)
            {
                // program name
                lpValue = strtok(NULL,lanToken);
                strcpy(sysConfig->lanChild2Proc,lpValue);
                
                // get port
                lpValue = strtok(NULL,lanToken);
                strcpy(sysConfig->lnChild2Port,lpValue);
		if(sysConfig->lnChild2Port[strlen(sysConfig->lnChild2Port)-1]=='\r')
			sysConfig->lnChild2Port[strlen(sysConfig->lnChild2Port)-1]='\0';
            }
            else if (strcmp(lpTag,"proc3") == 0)
            {
                // program name
                lpValue = strtok(NULL,lanToken);
                strcpy(sysConfig->lanChild3Proc,lpValue);
                
                // get port
                lpValue = strtok(NULL,lanToken);
                strcpy(sysConfig->lnChild3Port,lpValue);
		if(sysConfig->lnChild3Port[strlen(sysConfig->lnChild3Port)-1]=='\r')
		 sysConfig->lnChild3Port[strlen(sysConfig->lnChild3Port)-1]='\0';
            }
                
        }
        fclose(lpFile);
        
        printf("Server Address = %s",sysConfig->lanServerAddress);
        printf("Child1 Program Name = %s\n",sysConfig->lanChild1Proc);
        printf("Child1 Port = %s",sysConfig->lnChild1Port);
        printf("Child2 Program Name = %s\n",sysConfig->lanChild2Proc);
        printf("Child2 Port = %s",sysConfig->lnChild2Port);
        printf("Child3 Program Name = %s\n",sysConfig->lanChild3Proc);
        printf("Child3 Port = %s",sysConfig->lnChild3Port);

    }
    else 
        return -1;
    
    
    return 0;
}
