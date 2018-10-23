/*********************
CIS3210 assignment 1
Haiqing Gong
********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAXRCVLEN 4096
#define FILENAME "smallTest.txt"
 
int main(int argc, char *argv[])
{
	if (argc < 2 || argc > 3)
	{
		printf("Usage: %s server-IP-address:port-number\n", argv[0]);
		return EXIT_SUCCESS;
	}

	// read buffen length
	size_t bufferlen = MAXRCVLEN;
	if (argc == 3)
	{
		bufferlen = atoi(argv[2]);
		if (bufferlen > MAXRCVLEN)
		{
			bufferlen = MAXRCVLEN;
		}
	}

	char buffer[MAXRCVLEN + 1] = "this is a test"; /* +1 so we can add null terminator */
	int len, mysocket;
	struct sockaddr_in dest; // socket info about the machine connecting to us

	char ip[20];
	int port;
	int i;
	// parse ip and port
	for (i = 0; i < strlen(argv[1]) && argv[1][i] != ':'; i++)
	{
		ip[i] = argv[1][i];
	}

	port = atoi(argv[1] + i + 1);
 
	/* Create a socket.
	   The arguments indicate that this is an IPv4, TCP socket
	*/
	mysocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mysocket == -1)
	{
		perror("Error in socket");
		exit(EXIT_FAILURE);
	}
  
	memset(&dest, 0, sizeof(dest));                // zero the struct
	
	//Initialize the destination socket information
	dest.sin_family = AF_INET;					   // Use the IPv4 address family
	if (inet_aton(ip, &dest.sin_addr) == 0)        // Set destination IP number
	{
		perror("Error in inet_aton");
		exit(EXIT_FAILURE);
	}
	dest.sin_port = htons(port);                   // Set destination port number
 	
 	printf("connecting to the server\n");
	// Connect to the server
	if (connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr_in)) == -1)
	{
		printf("failed to connect\n");
		perror("Error in connect");
		exit(EXIT_FAILURE);
	}
  
	// Open file and read content of file
	FILE *file = fopen(FILENAME, "r");
	if (file == NULL)
	{
		perror("Error in fopen");
		exit(EXIT_FAILURE);
	}
	len = fread(buffer, 1, bufferlen, file) + strlen(FILENAME) + 1;
	char output[MAXRCVLEN + 1];
	
	

	while (len)
	{
		sprintf(output, "%s|%s", "test.txt",buffer);
		printf("%s\n", output);
		// send file content
		len = send(mysocket, output, len, 0);
		if (len == -1)
		{
			perror("Error in send");
		}
		// continue reading file content
		len = fread(buffer, 1, bufferlen, file);
		if(len){
			len = len  + strlen(FILENAME) + 1;
		}
	}

	close(mysocket);
	return EXIT_SUCCESS;
}
