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

struct map_unmap{
	char *path;
	int offset;
};
typedef struct map_unmap map_unmap;

struct read_rqst{
	char *path;
	int offset;
	int rd_count;
};
typedef struct read_rqst read_rqst;

struct write_rqst{
	char *path;
	int offset_file;
	int offset_write;
	char *data;
};
typedef struct write_rqst write_rqst;

int getInt(int socket_fd);
int getData(int socket_fd, char *buffer, int length);
map_unmap* mapUnmapRequest(int socekt_fd);
read_rqst* readRequest(int socekt_fd);
write_rqst* writeRequest(int socekt_fd);


#endif /* NET_PROTOCOL_H_ */
