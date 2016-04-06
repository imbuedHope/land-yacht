#pragma once

#define PORT_NO 50000

void comm_thread(void*);

/* A simple server in the internet domain using TCP */
static int server(int);

/* A simple I/O manage function
 * Returns 0 when one message is handled
 * Returns non 0 when message is handled & need to quit / error */
static int manage_server(int, char*);