/*********************
CIS3210 assignment 1
Haiqing Gong
********************/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <extraFunctions.h>
 
#define MAXRCVLEN 4096

typedef struct
{
	int consocket;
	int messageID;
}threadArgs;

void* UIThread(void* arg);
int serverState = 1;
void* recieveConnection(void* arg);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s port-number\n", argv[0]);
		return EXIT_SUCCESS;
	}
	int port = atoi(argv[1]);
	
	pthread_t uTid;//user interface thread id
	pthread_t * cTid = NULL;//un initialized list of thread ids for client connections
  
	struct sockaddr_in dest; // socket info about the machine connecting to us
	struct sockaddr_in serv; // socket info about our server
	int mysocket;            // socket used to listen for incoming connections
	socklen_t socksize = sizeof(struct sockaddr_in);

	memset(&serv, 0, sizeof(serv));           // zero the struct before filling the fields
	
	serv.sin_family = AF_INET;                // Use the IPv4 address family
	serv.sin_addr.s_addr = htonl(INADDR_ANY); // Set our address to any interface
	serv.sin_port = htons(port);              // Set the server port number

	/* Create a socket.
   	   The arguments indicate that this is an IPv4, TCP socket
	*/
	mysocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mysocket == -1)
	{
		perror("Error in socket");
		exit(EXIT_FAILURE);
	}
  
	// bind serv information to mysocket
	bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr));

	// start listening, allowing a queue of up to 1 pending connection
	listen(mysocket, 1);
	
	// Create a socket to communicate with the client that just connected
	pthread_create(&uTid, NULL, UIThread, NULL);
	TransferList* listOfThreads = createTransferList();
	/*
	addTransfer(listOfThreads, 100, "hello.txt", "this is data", 100, 1);
	displayTransferList(listOfThreads);*/
	//hands off stdinput to thread
	int consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);
	int numServiced = 1;
	threadArgs* tArgs = NULL;

	while (serverState)
	{
		//checks if it has stopped all connections
		if(serverState != 2){
			printf("Incoming connection from %s \n", inet_ntoa(dest.sin_addr));	
			cTid = (pthread_t*)realloc(cTid, sizeof(pthread_t)*(numServiced + 1));
			tArgs = (threadArgs*)realloc(tArgs, sizeof(threadArgs)*(numServiced + 1));
			tArgs[numServiced - 1].consocket = consocket;
			tArgs[numServiced - 1].messageID = numServiced;
			pthread_create(&cTid[numServiced], NULL, recieveConnection, &tArgs[numServiced - 1]);
			numServiced = numServiced + 1;
			// Continue listening for incoming connections
			consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);
		}	
	}

	free(cTid);
	free(tArgs);
	close(mysocket);
	return EXIT_SUCCESS;
}

void* UIThread(void* arg){
	char state = '0';
	system("/bin/stty raw");
	while(serverState){
		state = getchar();
		if(state == 'o'){
			//kill all
			printf("killing all\n");
			serverState = 0;
		}else if(state == 's'){
			printf("server stopping incoming connections\n");
			serverState = 2;;
		}else if(state == 'c'){
			printf("resuming connections\n");
			serverState = 1;
		}	
	}
	system ("/bin/stty cooked");
	kill(getpid(), SIGTERM);
	return NULL;
}

void* recieveConnection(void* arg){
	threadArgs* args = (threadArgs*)arg;
	int consocket =  args->consocket;
	int len = 0;
	char buffer[MAXRCVLEN + 1]; // +1 so we can add null terminator
	int filesuffix = 0;
	// Receive data from the client
	len = recv(consocket, buffer, MAXRCVLEN, 0);
	if (len == -1)
	{
		perror("Error in recv");
	}
	buffer[len] = '\0';
	printf("Receive %d bytes from client\n", len);
	printf("string length = %d\n", (int)strlen(buffer));
	// Create a file to write
	// Receive data from client, then write to file.
	char filename[30];
	/*sprintf(filename, "data.%d", filesuffix++);
	FILE *file = fopen(filename, "w");
	*/
	while (len)
	{
		//fwrite(buffer, 1, len, file);

		len = recv(consocket, buffer, MAXRCVLEN, 0);
		if (len == -1)
		{
			perror("Error in recv");
		}
		char* data = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(data, strpbrk(buffer, "|") + 1);
		strncpy(filename, buffer, strpbrk(buffer, "|") - buffer);
		printf("%s\n", filename);
		printf("%s\n", data);
		
		data[len] = '\0';
		if (len != 0)
		{
			printf("Receive %d bytes from client\n", len);
		}
	}
	
	printf("Receive successful.\n");

	// Close file
	//fclose(file);

	// Close current connection
	close(consocket);
	return NULL;
}