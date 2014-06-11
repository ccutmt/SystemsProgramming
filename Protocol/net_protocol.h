#ifndef NET_PROTOCOL_H_
#define NET_PROTOCOL_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define ACK 1
#define MAP 2
#define UNMAP 3
#define READ 4
#define WRITE 5
#define ERROR 6

struct rm_protocol{
	int type;
	int error_id;
	int path_length;
	char * path;
	int offset;
	int count;
	int data_length;
	char * data;
};
typedef struct rm_protocol rm_protocol;

int getInt(int socket_fd);
int getData(int socket_fd, char *buffer, int length);
int sendInt(int socket_fd, int tosend);
int sendData(int socket_fd, char *data, int length);
rm_protocol* readFromNet(int socket_fd);
void sendStruct(int socket_fd, rm_protocol *data);


#endif /* NET_PROTOCOL_H_ */
