#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#include "comms.h"

int main(int argc, char *argv[])
{

	signal(SIGPIPE, SIG_IGN); 
	
	pthread_t server_thread;

	if(pthread_create(&server_thread, NULL, (void *) &comm_thread, (void *) 0))
	{
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	if(pthread_join(server_thread, NULL))
	{
		fprintf(stderr, "Error joining thread\n");
		return 2;
	}

	return 0;
}
