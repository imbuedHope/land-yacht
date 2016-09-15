#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "server/comms.h"
#include "server/comn.h"

#define BUFFER_LEN 256

int sockfd;

/* A simple server in the internet domain using TCP */
static int server(int);

/* A simple I/O manage function
* Returns 0 when one message is handled
* Returns non 0 when message is handled & need to quit / error */
static int manage_server(int, char*);

// wrapper for pthread, see server function for actual handler
void* comm_thread(void* argv)
{
// ignore all inputs
// argv present only to satisfy casting

	server( PORT_NO );

	return (void*) 0;
}

static int manage_server(int sockfd, char* buffer)
{
	int n;

	while(1 && halt_system == RUN_STATE)
	{

		memset(buffer, 0, 255);
		n = read(sockfd, buffer, BUFFER_LEN);

		if (n < 0)
		{
			printf("Error reading from socket\n");
			return -1;
		}

		printf("client> %s",buffer);

		if (n < 0)
		{
			printf("> client left the connection, closing connection\n");
			return -1;
		}

		// Parse message from client here
		if(strncmp(buffer, "exit", 4) == 0)
		{
			// close client connection
			return 0;
		}

		if(strncmp(buffer, "halt", 4) == 0) 
		{
			// shutdown the server application
			halt_system = HALT_STATE;
			return 0;
		}

		// set a value in the comn_dat array
		// syntax: set_val ind val
		if(strncmp(buffer, "set_val", 7) == 0)
		{
			// set value in data buffer
			int k = strlen(buffer);
			for(int i = 0; i < k; i++)
				if(buffer[i] == ' ')
					buffer[i] = '\0';

			int ind = atoi(buffer+8);
			int val = atoi(buffer+9+strlen(buffer+8));

			if (ind > COMN_DAT_LEN || ind < 0)
				continue;

			pthread_mutex_lock(&comn_mutex);

			comn_dat[ind] = val;

			pthread_mutex_unlock(&comn_mutex);
			continue;
		}
		
		// dump array data onto socket
		if(strncmp(buffer, "poll", 4) == 0)
		{
			// dump values in data buffer
			for(int i = 0; i < COMN_DAT_LEN; i++)
			{

				pthread_mutex_lock(&comn_mutex);

				snprintf(buffer, BUFFER_LEN, "%d: %d\n", i, comn_dat[i]);
			 	n = write(sockfd, buffer, strlen(buffer));

				pthread_mutex_unlock(&comn_mutex);

				if (n < 0)
				{
					perror("Error writing to socket");
					return -1;
				}
			}

			continue;
		}

		buffer[0] = ' ';
		buffer[1] = '\0';
		if ( write(sockfd, buffer, strlen(buffer)) < 0 )
		{
			perror("Error writing to socket");
			return -1;
		}
	}

	return -1;
}

// initalizle server, wait for client, and handle client
static int server(int portno)
{
	int newsockfd;
	socklen_t clilen;
	char buffer[BUFFER_LEN];
	struct sockaddr_in serv_addr, cli_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("> error opening socket\n");
		return -1;
	}

	memset((char *) &serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("> error on binding - port may be in use\n");
		return -1;
	}

	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	while(halt_system == RUN_STATE)
	{
		printf("> wating for client new connection\n");
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
		{
			printf("> error on accept\n");
			return -1;
		}

		// NOTE: could fork right here to allow for multiple client connections
		// manage_server handles actually parsing messages from the client and is a blocking function
		manage_server(newsockfd, buffer);
		close(newsockfd);

		printf("> closed connection");
	}

	close(sockfd);
	return 0; 
}
