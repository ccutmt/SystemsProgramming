#include "tcp_server.h"
<<<<<<< HEAD
#include "ArrayList.h"
#include <errno.h>
#include "files.h"

#define ACK 1 
#define MAP 2 
#define UNMAP 3 
#define READ 4
#define WRITE 5 
#define ERROR 6
=======
>>>>>>> 4fa160a4e2221f83d6864262a38aab32b479afff

ArrayList * Files = NULL;

int main( int argc, char *argv[] )
{
	// Declare variables
	//ArrayList * ClientList = NULL;
	fd_set readset;
	int sockfd, newsockfd, portno, clilen;
	struct sockaddr_in serv_addr, cli_addr;

	// Create server socket (AF_INET, SOCK_STREAM)
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("ERROR opening socket");
		exit(1);
	}

	// Initialize socket structure
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 8080; // Server port
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY; // Accept connections from any address
	serv_addr.sin_port = htons(portno);

	// Bind the host address
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
		exit(1);
	}

<<<<<<< HEAD
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
    
    char *file_name = "FILE_1.txt";
    //printf("\nEnter name of file: ");
    //scanf("%s", file_name);
    if (openFile(file_name) > 0)
    {
	printf("File Opened.\n");
    }
    else
	printf("Error on opening.\n");
	
    file *f = (file*) getElement(file, 0);
    if(closeFile(f->fd) > 0)
	printf("File Closed.\n");
    else
	printf("Error on closing.\n");
    
    // All done, close sockets
    //close(newsockfd);
    //close(sockfd);
    return 0; 
}

/*void openFile(char f[])
{
   if(Files == NULL)
   {
	Files = malloc(sizeof(ArrayList));
	initArrayList(Files);
    }
    file * file = malloc(sizeof(file));
=======
	// Start listening for the clients, here process will
	// go in sleep mode and will wait for the incoming connection
	listen(sockfd,10);
	clilen = sizeof(cli_addr);

	//initialize listening set
	FD_ZERO(&readset);
	FD_SET(sockfd, &readset);
	//printf("Sockfd: %i", sockfd);
>>>>>>> 4fa160a4e2221f83d6864262a38aab32b479afff

	//printf("before loop");
	maxfd = sockfd;

	// Accept actual connection from the client
	int i = 0;
	for(;;){
		fflush(stdout);
		int result = select(maxfd + 1, &readset, NULL, NULL, NULL);
		fflush(stdout);
		if(result > 0){
			for(i = 2; i < maxfd+1; i++){
				if(FD_ISSET(i, &readset)){
					if(i == sockfd){
						fflush(stdout);
						newsockfd = acceptConnection(sockfd, &readset);//(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*) &clilen);
						if (newsockfd < 0)
						{
							exit(1);
						}
					}else{
						//printf("result: %i", result);
						rm_protocol *message = malloc(sizeof(rm_protocol));
						if(readFromNet(i, message) != -1){
							printf(
									"pid: %i\nFilepart: %u\nType: %i\nErrorId: %i\nPath: %s\ncount: %i\nData: %s\n",
									message->pid, message->filepart, message->type, message->error_id,
									message->path, message->count, message->data);
							fflush(stdout);
							sendStruct(i, message);
						}else{
							printf("closing %i\nmaxfd: %i", i, maxfd);
							fflush(stdout);

							FD_CLR(i, &readset);
							FD_SET(sockfd, &readset);
							close(i);
						}
					}
				}
			}
		}
	}

	/*else
{
	printf("Client connected\n");
	/*	unsigned long ip = cli_addr.sin_addr.s_addr;
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
	printf("Client details\nIP: %lu \n ", e->ip);*/
	//}

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

<<<<<<< HEAD
void closeFile(FILE * f)
{
	fclose(f);
}*/
=======
	/* char file_name[10];
printf("\nEnter name of file: ");
scanf("%s", file_name);
openFile(file_name);*/

	// All done, close sockets
	//close(newsockfd);
	close(sockfd);
	return 0;
}
>>>>>>> 4fa160a4e2221f83d6864262a38aab32b479afff

int acceptConnection(int socket, fd_set *listening){
	struct sockaddr_in *cli_addr = malloc(sizeof(struct sockaddr_in));
	int size = sizeof(struct sockaddr_in);
	int fd = accept(socket, (struct sockaddr*)cli_addr, (socklen_t*)&size);
	if(fd == -1){
		perror("Accept failed");
		return -1;
	}
	else{
		FD_SET(fd, listening);
		if(fd > maxfd)
			maxfd = fd;
		return fd;
	}
}
