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

char halt_system = RUN_STATE;
int comn_dat[COMN_DAT_LEN];

pthread_mutex_t comn_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[])
{

	signal(SIGPIPE, SIG_IGN); 
	
	pthread_t server_thread;
	pthread_t action_thread;

	if(pthread_create(&server_thread, NULL, &comm_thread, (void *) 0))
	{
		fprintf(stderr, "> error creating server thread\n");
		return 1;
	}

	if(pthread_create(&action_thread, NULL, &control_thread, (void *) 0))
	{
		fprintf(stderr, "> error creating controls thread\n");
		return 1;
	}

	// this thread will sleep and the 2 child threads will do the work

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
