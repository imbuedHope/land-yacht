# a magical little Makefile -- here's hoping it works!

CERR = -Wall -Wextra
CFLAGS = -std=c99 -pthread
CPPFLAGS = -lstdc++ -pthread 
CC = gcc
SERVER_DIR = src/server/
CLIENT_DIR = src/client/
LIB_DIR = src/lib/

server: build/server build/lib
	g++ $(CPPFLAGS) -o server $(wildcard build/server/*) $(wildcard build/lib/*)

build/server:
	mkdir -p build/server
	cd $(SERVER_DIR); \
	gcc -c $(CERR) $(CFLAGS) -I .. *.c; \
	gcc -c $(CERR) $(CPPFLAGS) -I .. *.cpp;
	mv $(SERVER_DIR)*.o build/server/

client: build/client
	gcc $(CFLAGS) -o client $(wildcard build/client/*)

build/client:
	mkdir -p build/client/
	cd $(CLIENT_DIR); \
	gcc -c $(CERR) $(CFLAGS) -I .. *.c;
	mv $(CLIENT_DIR)*.o build/client/


build/lib:
	mkdir -p build/lib/
	cd $(LIB_DIR); \
	gcc -c $(CERR) $(CPPFLAGS) -I .. *.cpp;
	mv $(LIB_DIR)*.o build/lib/

clean:
	rm -rf build/ server client