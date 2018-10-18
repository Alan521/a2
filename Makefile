CC = gcc
CFLAGS = -Wall -g

all: client server

client: TCPclient.c TCPclient.c
	$(CC) $(CFLAGS) -o client TCPclient.c 
	
server: TCPserver.c TCPserver.c
	$(CC) $(CFLAGS) -o server TCPserver.c -lpthread

clean:
	rm client server