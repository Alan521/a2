CC = gcc
CFLAGS = -Wall -g

all: client server

client: TCPclient.c TCPclient.c
	$(CC) $(CFLAGS) -o client TCPclient.c 
	
server: TCPserver.c TCPserver.c extraFunctions.c
	$(CC) $(CFLAGS) -I./ -o server extraFunctions.c TCPserver.c  -lpthread 

clean:
	rm client server