#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include "../Protocol/net_protocol.h"

#define ACK 1 
#define MAP 2 
#define UNMAP 3 
#define READ 4
#define WRITE 5 
#define ERROR 6

int main( int argc, char *argv[] )
{
    // Declare variables
    int sockfd, newsockfd, portno, clilen;
    //char buffer[256];
    //char s_buf[1];
    struct sockaddr_in serv_addr, cli_addr;
    //int32_t  n;

    // Create server socket (AF_INET, SOCK_STREAM)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("ERROR opening socket");
        exit(1);
    }

    // Initialize socket structure
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 8080;  // Server port
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // Accept connections from any address
    serv_addr.sin_port = htons(portno);
 
    // Bind the host address
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
                          sizeof(serv_addr)) < 0)
    {
         perror("ERROR on binding");
         exit(1);
    }

    // Start listening for the clients, here process will
    // go in sleep mode and will wait for the incoming connection
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    // Accept actual connection from the client
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*) &clilen);
    if (newsockfd < 0) 
    {
        perror("ERROR on accept");
        exit(1);
    }
	else
	{
		printf("Client connected\n");
	}

    // If connection is established then start communicating 
    /*bzero(buffer,256);
    int32_t rec_int = 0;
    n = read(newsockfd, &rec_int, sizeof(rec_int));
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }*/

<<<<<<< HEAD
    rm_protocol *message;
    //message = readFromNet(newsockfd);
    //printf("%i %i", message->type, message->offset);
=======
    /*
     * Read from network
     */
    /*rm_protocol *message;
    message = readFromNet(newsockfd);
    printf("%i %i", message->type, message->offset);*/
>>>>>>> 42c1fe666b4fc8f52390abbbe1b368112433f7b5


    //printf("Here is the message: %i\n",(int) ntohl(rec_int));

    	// Write a response to the client
	/*int test = 1;
	int32_t converted_number = htonl(test); 
	n = write(newsockfd, &converted_number, sizeof(converted_number)); 
	if (n < 0) 
	{ 
		perror("ERROR writing to socket"); 
		exit(1); 
	}*/

    // All done, close sockets
    close(newsockfd);
    close(sockfd);
    return 0; 
}
