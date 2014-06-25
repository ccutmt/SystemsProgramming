#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include "../Protocol/net_protocol.h"
#include "tcp_server.h"
#include "ArrayList.h"
#include <errno.h>

#define ACK 1 
#define MAP 2 
#define UNMAP 3 
#define READ 4
#define WRITE 5 
#define ERROR 6

ArrayList * Files = NULL;

int main( int argc, char *argv[] )
{
    // Declare variables
    /*ArrayList * ClientList = NULL;
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    char s_buf[1];
    struct sockaddr_in serv_addr, cli_addr;
    int32_t  n;*/

    // Create server socket (AF_INET, SOCK_STREAM)
    /*sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("ERROR opening socket");
        exit(1);
    }*/

    // Initialize socket structure
    /*bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 8080;  // Server port
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // Accept connections from any address
    serv_addr.sin_port = htons(portno);*/
    // Bind the host address
    /*if (bind(sockfd, (struct sockaddr *) &serv_addr,
                          sizeof(serv_addr)) < 0)
    {
         perror("ERROR on binding");
         exit(1);
    }*/
    // Start listening for the clients, here process will
    // go in sleep mode and will wait for the incoming connection
    /*listen(sockfd,5);
    clilen = sizeof(cli_addr);*/

    // Accept actual connection from the client
    /*newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*) &clilen);
    if (newsockfd < 0) 
    {
        perror("ERROR on accept");
        exit(1);
    }
	else
	{
		printf("Client connected\n");		
    		unsigned long ip = cli_addr.sin_addr.s_addr;
		printf("Client details\nIP: %lu \n", ip);
		if(ClientList == NULL)
		{
			ClientList = malloc(sizeof(ArrayList));
			initArrayList(ClientList);
		}
		connections * conn = malloc(sizeof(connections));
		conn->ip = ip;
		conn->id = 0;
		add(ClientList, conn);
		connections *e = (connections*) getElement(ClientList, 0);
		printf("Client details\nIP: %lu \n ", e->ip);
	}*/

    // If connection is established then start communicating 
    /*rm_protocol *message;
    message = readFromNet(newsockfd);
    printf("%i %i %i %s", message->type, message->offset, message->data_length ,message->data);*/
    /*
     * Read from network
     */
    /*rm_protocol *message;
    message = readFromNet(newsockfd);
    printf("%i %i", message->type, message->offset);*/
    
    char file_name[10];
    printf("\nEnter name of file: ");
    scanf("%s", file_name);
    openFile(file_name);
    
    // All done, close sockets
    //close(newsockfd);
    //close(sockfd);
    return 0; 
}

void openFile(char f[])
{
   if(Files == NULL)
   {
	Files = malloc(sizeof(ArrayList));
	initArrayList(Files);
    }
    file * file = malloc(sizeof(file));

    FILE *fp;
    char fread_buffer[20];
    char c[] = "Testing files";
    fp = fopen(f, "w+");

    int errno;
    if(fp == NULL)
    {
      	fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    }
    else
    {
    	file->fid++;
    	file->fd = fp;
    	add(Files, file);

	//Writing to a file
	fwrite(c, strlen(c) + 1, 1, fp);

	//Setting the pointer to position 
	fseek(fp, 0, SEEK_SET);

	//Reading from a file
    	fread(fread_buffer, sizeof(c), 1, fp);
    	printf("\nRead Buffer: %s\n", fread_buffer);
   	closeFile(fp);
    }	
}

void closeFile(FILE * f)
{
	fclose(f);
}

/*void readFile(FILE *f, int offset)
{
	fseek(f, offset, SEEK_SET);
	
	fread(
}*/
