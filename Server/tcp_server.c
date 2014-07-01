#include "tcp_server.h"

ArrayList * Files = NULL;

int main( int argc, char *argv[] )
{
	// Declare variables
	//ArrayList * ClientList = NULL;
	fd_set readset;
	int sockfd, newsockfd, portno;
	struct sockaddr_in serv_addr;

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

	// Start listening for the clients, here process will
	// go in sleep mode and will wait for the incoming connection
	listen(sockfd,10);

	//initialize listening set
	FD_ZERO(&readset);
	FD_SET(sockfd, &readset);

	initConnections();
	initDataStructure();
	//printf("Sockfd: %i", sockfd);

	//printf("before loop");
	maxfd = sockfd;

	// Accept actual connection from the client
	int i = 0;
	for(;;){
		fflush(stdout);
		int result = select(maxfd + 1, &readset, NULL, NULL, NULL);
		if(result > 0){
			for(i = 2; i < maxfd+1; i++){
				if(FD_ISSET(i, &readset)){
					if(i == sockfd){
						newsockfd = acceptConnection(sockfd, &readset);//(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*) &clilen);
						if (newsockfd < 0)
						{
							exit(1);
						}
					}else{
						//printf("result: %i", result);
						rm_protocol *message = malloc(sizeof(rm_protocol));
						rm_protocol *reply = malloc(sizeof(rm_protocol));
						if(readFromNet(i, message) != -1){
							/*printf(
									"pid: %i\nFilepart: %u\nType: %i\nErrorId: %i\nPath: %s\ncount: %i\nData: %s\n",
									message.pid, message.filepart, message.type, message.error_id,
									message.path, message.count, message.data);
							fflush(stdout);*/

							manager(message, reply, i);
							printf("Reply: %s", reply->data);
							sendStruct(i, reply);
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
	close(sockfd);
	return 0;
}

int acceptConnection(int socket, fd_set *listening){
	struct sockaddr_in cli_addr;
	int size = sizeof(struct sockaddr_in);
	int fd = accept(socket, (struct sockaddr*)&cli_addr, (socklen_t*)&size);
	if(fd == -1){
		perror("Accept failed");
		return -1;
	}
	else{
		addConnection(fd, cli_addr.sin_addr.s_addr);
		FD_SET(fd, listening);
		if(fd > maxfd)
			maxfd = fd;
		return fd;
	}
}

void manager(rm_protocol *received, rm_protocol * reply, int fd){
	connection *conn = getConnectionByFd(fd);
	switch(received->type){
	case MAP:{
		mapRequest(received->path, received->offset, conn, reply->data);
		break;
	}
	case READ:{
		readRequest(received->filepart, received->offset, _DATA_LENGTH, reply->data, conn);
		break;
	}
	case WRITE:{
		writeRequest(received->filepart, received->offset, _DATA_LENGTH, received->data, conn);
		break;
	}
	case UNMAP:{
		unmapRequest(received->filepart, received->offset, conn);
		break;
	}
	}
}
