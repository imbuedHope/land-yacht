#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

extern "C"
{
#include "server/comms.h"
#include "server/comn.h"
}
#include "server/control.h"
	
pthread_t server_thread;
pthread_t action_thread;

volatile char halt_system = RUN_STATE;
volatile int comn_dat[COMN_DAT_LEN];

pthread_mutex_t comn_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[])
{
	//ignore SIGPIPE signals; this means that the client crashed -- ignoring is fine for now
	signal(SIGPIPE, SIG_IGN); 
	
	// launch thread to handle communication with a client; see: comms.c
	if(pthread_create(&server_thread, NULL, &comm_thread, (void *) 0))
	{
		fprintf(stderr, "> error creating server thread\n");
		return 1;
	}

	// launch thread to handle mechnical controls; see: controls.cpp
	if(pthread_create(&action_thread, NULL, &control_thread, (void *) 0))
	{
		fprintf(stderr, "> error creating controls thread\n");
		return 1;
	}

	// this thread will sleep and the 2 child threads will do the work
	// close threads and end the server process when they end
	if(pthread_join(server_thread, NULL))
	{
		fprintf(stderr, "> error joining server thread\n");
		return 2;
	}

	if(pthread_join(action_thread, NULL))
	{
		fprintf(stderr, "> error joining controls thread\n");
		return 2;
	}

	return 0;
}
