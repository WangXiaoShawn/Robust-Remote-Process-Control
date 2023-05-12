
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "../comm/configreader.h"
// structure to hold the parameters that are sent to the signal handling thread
typedef struct tp
{
    int lnSignal;
    siginfo_t * lpSigInfo;
} ThreadParams;

typedef struct ci{
	int pid;
	char name[256];
	char port[256];
}ChildInfo;

// prototypes
int forkexample(ChildInfo ch, char ip[]);
void *SignalThread( void *ptr );
void forkbyId(int pid);

// our signal handler
void SigHandler(int sig, siginfo_t *info, void *context)
{
    // need to dynamically allocate memory, which must exist AFTER
    // signal handler exists
    // Thread MUST delete memory to prevent memory leak
    ThreadParams * lpThreadParams = (ThreadParams*) malloc(sizeof(ThreadParams));
    lpThreadParams->lnSignal = sig;
    lpThreadParams->lpSigInfo = info;
	// we don't uset the context. it is for advanced signal handling, but
    // we need to do something with it to get rid of the unused parameter compiler warning
    context = context;

    // create a thread to handle the signal
    pthread_t thread1;
    int iret1 = pthread_create( &thread1, NULL, SignalThread, (void*) lpThreadParams);
    if (iret1 !=0)
    {
            printf("Unable to create thread. Exiting");
            exit(-1); // exiting ensures any threads that did start will be terminated
    }
}

SystemConfig syscon;
ChildInfo children[3];
	
int main()
{
    // install the signal handler for SIGCHLD
    // use the sigaction library function instead of signal to get more information
    // about the process that sent the signal
    // also, signal is deprecated
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO; // indicates siginfo struct is sent to handler
    sa.sa_sigaction = SigHandler;
    sigaction(SIGCHLD, &sa, NULL);

	
	GetSystemConfig(&syscon);

	strcpy(children[0].name,syscon.lanChild1Proc);	strcpy(children[0].port,syscon.lnChild1Port);
	strcpy(children[1].name,syscon.lanChild2Proc);	strcpy(children[1].port,syscon.lnChild2Port);
	strcpy(children[2].name,syscon.lanChild3Proc);	strcpy(children[2].port,syscon.lnChild3Port);
	for(int i=0;i<3;i++){
		children[i].pid=forkexample(children[i],syscon.lanServerAddress);
	}

    printf("Inside Parent with PID = %d\n", getpid());
        // do nothing waiting to be terminated
    for(int i=1;;i++)
    {    //Infinite loop
		fflush(stdout);
        sleep(1);  // Delay for 1 second
    }
    return 0;
}

int forkexample(ChildInfo ch,char ip[])
{
    // fork a child process
    int lnPid = fork();

    // child process because return value zero
    if ( lnPid == 0)
    {
        // null terminated list of arguments, where the first argument must be the namespace
        // of the program
        char * args[] = {ch.name,ip,ch.port,NULL};
        int status = execvp(args[0],args);
       // printf("Should not get here if execv was successful: %d\n",status);
		exit(-1);
    }

    // parent process because the value is > 0
    // the value returned is the PID of the child that was created
    else if ( lnPid > 0 )
    {
        printf("Hello from Parent. My PID = %d!\n",getpid());
        printf("Started child with PID = %d\n",lnPid);
    }
    else
    {
        //perror("Failed to start child with fork\n");
		exit(-1);
	}
	return lnPid;
}


void *SignalThread( void *ptr )
{
    // cast our input argument to the correct type that we are expecting
    ThreadParams * lpThreadParams = (ThreadParams*) ptr;

    switch(lpThreadParams->lnSignal)
    {
        case SIGINT:
        printf("Child %d killed from process %d\n", getpid(), lpThreadParams->lnSignal, lpThreadParams->lpSigInfo->si_pid);
        break;

        case SIGCHLD:
        printf("Child %d killed from process %d\n", getpid(), lpThreadParams->lnSignal, lpThreadParams->lpSigInfo->si_pid);
		forkbyId(lpThreadParams->lpSigInfo->si_pid);
        break;

        default:
        printf("Child %d receivd signal %d from process %d\n", getpid(), lpThreadParams->lnSignal, lpThreadParams->lpSigInfo->si_pid);
    }

    // per comment in Signal Handler, the memory must be deleted when
    // we are done with it
    free(lpThreadParams);

    return NULL;


}

void forkbyId(int pid){
	for(int i=0;i<3;i++){
		if(children[i].pid==pid){
			children[i].pid=forkexample(children[i],syscon.lanServerAddress);
			return;
		}
	}
	return;
}
