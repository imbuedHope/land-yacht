#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

extern "C"
{
include "comms.h"
}

int main(int argc, char *argv[])
{

	signal(SIGPIPE, SIG_IGN); 
	
	pthread_t server_thread;
	// pthread_t controls_thread;

	if(pthread_create(&server_thread, NULL, (void *) &comm_thread, (void *) 0))
	{
		fprintf(stderr, "> error creating server thread\n");
		return 1;
	}

	// if(pthread_create(&controls_thread, NULL, (void *) &foo, (void *) bar))
	// {
	// 	fprintf(stderr, "> error creating controls thread\n");
	// 	return 1;
	// }

	if(pthread_join(server_thread, NULL))
	{
		fprintf(stderr, "> error joining server thread\n");
		return 2;
	}

	// if(pthread_join(controls_thread, NULL))
	// {
	// 	fprintf(stderr, "> error joining controls thread\n");
	// 	return 2;
	// }

	return 0;
}
