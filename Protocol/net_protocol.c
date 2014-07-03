#include "net_protocol.h"
#include <sys/types.h>

ArrayList *servers_mapped = NULL;

uint32_t getInt(int socket_fd) {
	uint32_t x;
	if (read(socket_fd, &x, sizeof(x)) <= 0) {
		return -1;
	}
	x = ntohl(x);
	return x;
}

int sendInt(int socket_fd, uint32_t tosend){
	int result = 0;
	int32_t converted_number = htonl(tosend);
	result = write(socket_fd, &converted_number, sizeof(converted_number));
	return result;
}

int getData(int socket_fd, char* buffer) {
	if (read(socket_fd, buffer, _MAXLENGTH) < 0) {
		perror("Error reading data from socket");
		return -1;
	}
	return 0;
}

int sendData(int socket_fd, char *data){
	int result = 0;
	result = write(socket_fd, data, _MAXLENGTH * sizeof(char));
	return result;
}


int readFromNet(int socket_fd, rm_protocol* mystruct) {
	int res = mystruct->pid = getInt(socket_fd);
	if(res == -1)
		return -1;
	mystruct->filepart = getInt(socket_fd);
	mystruct->type = getInt(socket_fd);
	mystruct->error_id = getInt(socket_fd);
	getData(socket_fd, mystruct->path);
	mystruct->offset = getInt(socket_fd);
	mystruct->count = getInt(socket_fd);
	getData(socket_fd, mystruct->data);
	return 0;
}

void sendStruct(int socket_fd, rm_protocol *data) {
	sendInt(socket_fd, data->pid);
	sendInt(socket_fd, data->filepart);
	sendInt(socket_fd, data->type);
	sendInt(socket_fd, data->error_id);
	sendData(socket_fd, data->path);
	sendInt(socket_fd, data->offset);
	sendInt(socket_fd, data->count);
	sendData(socket_fd, data->data);
}



int getServerFd(struct in_addr ip, int port){
	if(servers_mapped == NULL){
		servers_mapped = malloc(sizeof(ArrayList));
		initArrayList(servers_mapped);
	}

	int count = 0;
	server_data* temp;
	while(count <= servers_mapped->current){
		temp = (server_data*)getElement(servers_mapped,count);
		if(temp->ip == ip.s_addr){
			return temp->fd;
		}
		count++;
	}

	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_fd < 0)
		perror("Could not create socket");

	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = ip.s_addr;

	if ((connect(socket_fd, (struct sockaddr *) &addr, sizeof(addr))) < 0) {
		perror("Connection failed");
		return -1;
	}

	server_data *new = malloc(sizeof(server_data));
	new->fd = socket_fd;
	new->ip = ip.s_addr;

	add(servers_mapped, new);
	return socket_fd;
}

void makeMapRequest(rm_protocol *tosend, int pid, char *path, int offset){
	tosend->count = 0;
	tosend->error_id = 0;
	tosend->filepart = 0;
	tosend->offset = offset;
	memcpy(tosend->path, path, _MAXLENGTH * sizeof(char));
	tosend->pid = pid;
	tosend->type = MAP;
}

void makeReadRequest(rm_protocol *tosend, int pid, uint32_t filepartid, int offset, int count){
	tosend->count = count;
	tosend->error_id = 0;
	tosend->filepart = filepartid;
	tosend->offset = offset;
	tosend->pid = pid;
	tosend->type = READ;
}
void makeWriteRequest(rm_protocol *tosend, uint32_t fpart, int pid, char *data, int offset, int count){
	tosend->count = count;
	memcpy(tosend->data, data, _MAXLENGTH * sizeof(char));
	tosend->error_id = 0;
	tosend->filepart = fpart;
	tosend->offset = offset;
	tosend->pid = pid;
	tosend->type = WRITE;
}
void makeUnmapRequest(rm_protocol *tosend, uint32_t fpart, int pid){
	tosend->count = 0;
	tosend->error_id = 0;
	tosend->filepart = fpart;
	tosend->offset = 0;
	tosend->pid = pid;
	tosend->type = UNMAP;
}

