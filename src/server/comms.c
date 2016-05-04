#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

// might need later... see: https://computing.llnl.gov/tutorials/pthreads/#Management
// #include <pthread.h>

#include "comms.h"

//======================================================================

/* A simple server in the internet domain using TCP */
static int server(int);

/* A simple I/O manage function
* Returns 0 when one message is handled
* Returns non 0 when message is handled & need to quit / error */
static int manage_server(int, char*);

//======================================================================

void comm_thread(void* argv)
{
// ignore all inputs
// argv present only to satisfy casting

	server( PORT_NO );
}

static int manage_server(int sockfd, char* buffer)
{
	int n;

	memset(buffer, 0, 256);
	n = read(sockfd,buffer,255);

	if (n < 0)
	{
		printf("Error reading from socket\n");
		return -1;
	}

// handle input from client
	printf("> %s",buffer);

// respond with ack, inv, etc. 
	n = write(sockfd, "ack", 3);

	if (n < 0)
	{
		printf("Error writing to socket -- client probably closed\n");
		return -1;
	}

//TODO: Message handling somewhere around here in this function

//TODO: Replace "ext" with a more value
	if(strcmp(buffer, "ext") == 0)
		return -1;
	else
		return 0;
}

static int server(int portno)
{
	int sockfd, newsockfd;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("Error opening socket\n");
		return -1;
	}

	memset((char *) &serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("Error on binding - port may be in use\n");
		return -1;
	}

	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	while(1)
	{
		printf("wating for new connection");
// TODO: consider security here?
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
		{
			printf("Error on accept\n");
			return -1;
		}

// NOTE: could fork right here to allow for multiple client connections
		while(manage_server(newsockfd, buffer) == 0);
		close(newsockfd);

		printf("closed connection");
	}

	close(sockfd);
	return 0; 
}
