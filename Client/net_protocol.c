#include "net_protocol.h"
#include <sys/types.h>

int getInt(int socket_fd) {
	int32_t x;
	if (read(socket_fd, &x, sizeof(x)) <= 0) {
		perror("Error reading from socket");
		return -1;
	}
	x = ntohl(x);
	return x;
}

int getData(int socket_fd, char* buffer, int length) {
	if (read(socket_fd, buffer, length) <= 0) {
		perror("Error reading from socket");
		return -1;
	}
	return 0;
}

map_unmap* mapUnmapRequest(int socket_fd) {
	map_unmap *mystruct = malloc(sizeof(map_unmap));

	int32_t x = getInt(socket_fd);
	char *path = malloc(sizeof(char*) * x);
	getData(socket_fd, path, x);
	x = getInt(socket_fd);

	mystruct->offset = x;
	mystruct->path = path;
	return mystruct;
}

read_rqst* readRequest(int socket_fd) {
	read_rqst *mystruct = malloc(sizeof(read_rqst));

	int32_t x = getInt(socket_fd);
	char *path = malloc(sizeof(char*) * x);
	getData(socket_fd, path, x);
	int32_t offset = getInt(socket_fd);
	int32_t count = getInt(socket_fd);

	mystruct->offset = offset;
	mystruct->path = path;
	mystruct->rd_count = count;
	return mystruct;
}

write_rqst* writeRequest(int socket_fd) {
	write_rqst *mystruct = malloc(sizeof(write_rqst));

	int32_t x = getInt(socket_fd);
	char *path = malloc(sizeof(char*) * x);
	getData(socket_fd, path, x);
	int32_t offset_map = getInt(socket_fd);
	int32_t offset_rqst = getInt(socket_fd);
	int32_t d_length = getInt(socket_fd);
	char *data = malloc(sizeof(char*) * d_length);
	getData(socket_fd, data, d_length);

	mystruct->offset_file = offset_map;
	mystruct->offset_write = offset_rqst;
	mystruct->path = path;
	mystruct->data = data;
	return mystruct;
}
