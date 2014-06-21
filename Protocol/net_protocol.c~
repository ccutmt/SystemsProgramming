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

int sendInt(int socket_fd, int tosend){
	int result = 0;
	int32_t converted_number = htonl(tosend);
	result = write(socket_fd, &converted_number, sizeof(converted_number));
	return result;
}

int getData(int socket_fd, char* buffer, int length) {
	if (read(socket_fd, buffer, length) <= 0) {
		perror("Error reading from socket");
		return -1;
	}
	return 0;
}

int sendData(int socket_fd, char *data, int length){
	int result = 0;
	result = write(socket_fd, data, length * sizeof(char));
	return result;
}


rm_protocol* readFromNet(int socket_fd) {
	rm_protocol *mystruct = malloc(sizeof(rm_protocol));

	mystruct->type = getInt(socket_fd);
	mystruct->error_id = getInt(socket_fd);
	mystruct->path_length = getInt(socket_fd);
	char *path = malloc(sizeof(char*) * mystruct->path_length);
	getData(socket_fd, path, mystruct->path_length);
	mystruct->path = path;
	mystruct->offset = getInt(socket_fd);
	mystruct->count = getInt(socket_fd);
	mystruct->data_length = getInt(socket_fd);
	char *data = malloc(sizeof(char*) * mystruct->data_length);
	getData(socket_fd, path, mystruct->data_length);
	mystruct->data = data;

	return mystruct;
}

void sendStruct(int socket_fd, rm_protocol *data) {
	sendInt(socket_fd, data->type);
	sendInt(socket_fd, data->error_id);
	sendInt(socket_fd, data->path_length);
	sendData(socket_fd, data->path, data->path_length);
	sendInt(socket_fd, data->offset);
	sendInt(socket_fd, data->count);
	sendInt(socket_fd, data->data_length);
	sendData(socket_fd, data->data, data->data_length);
}
