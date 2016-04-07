# a magical little Makefile -- here's hoping it works!

CFLAGS = -Wall -std=c99 -pthread
CC = gcc
SERVER_DIR= src/server/
CLIENT_DIR= src/client/

server: build_s
	gcc $(CFLAGS) -o server $(wildcard build_s/*)

build_s:
	mkdir build_s/
	cd $(SERVER_DIR); \
	gcc -c $(CFLAGS) *.c;
	mv $(SERVER_DIR)*.o build_s/

client: build_c
	gcc $(CFLAGS) -o client $(wildcard build_c/*)

build_c:
	mkdir build_c/
	cd $(CLIENT_DIR); \
	gcc -c $(CFLAGS) *.c;
	mv $(CLIENT_DIR)*.o build_c/

clean:
	rm -rf build_s/ build_c/ server client