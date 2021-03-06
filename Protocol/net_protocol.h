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
#include "../ArrayList/ArrayList.h"

#define ACK 1
#define MAP 2
#define UNMAP 3
#define READ 4
#define WRITE 5
#define ERROR 6
#define _MAXLENGTH 256

struct rm_protocol{
	int pid;
	uint32_t filepart;
	int type;
	int error_id;
	char path[256];
	int offset;
	int count;
	int actual_read;
	char data[256];
};
typedef struct rm_protocol rm_protocol;

struct server_data{
	uint32_t ip;
	int fd;
};
typedef struct server_data server_data;

uint32_t getInt(int socket_fd);
int getData(int socket_fd, char *buffer);
int sendInt(int socket_fd, uint32_t tosend);
int sendData(int socket_fd, char *data);
int readFromNet(int socket_fd, rm_protocol* );
void sendStruct(int socket_fd, rm_protocol *data);

void makeMapRequest(rm_protocol *tosend, int pid, char *path, int offset);
void makeReadRequest(rm_protocol *tosend, int pid, uint32_t filepartid, int offset, int count);
void makeWriteRequest(rm_protocol *tosend, uint32_t fpart, int pid, char *data, int offset, int count);
void makeUnmapRequest(rm_protocol *tosend, uint32_t fpart, int pid);

int getServerFd(struct in_addr ip, int port);


#endif /* NET_PROTOCOL_H_ */
