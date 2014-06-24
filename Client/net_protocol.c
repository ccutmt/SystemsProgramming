#include "net_protocol.h"
#include <sys/types.h>

ArrayList *servers_mapped = NULL;

uint32_t getInt(int socket_fd) {
	uint32_t x;
	if (read(socket_fd, &x, sizeof(x)) <= 0) {
		perror("Error reading from socket");
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
	if (read(socket_fd, buffer, 256) < 0) {
		perror("Error reading data from socket");
		return -1;
	}
	return 0;
}

int sendData(int socket_fd, char *data){
	int result = 0;
	result = write(socket_fd, data, 256 * sizeof(char));
	return result;
}


void readFromNet(int socket_fd, rm_protocol* mystruct) {
	mystruct->pid = getInt(socket_fd);
	mystruct->filepart = getInt(socket_fd);
	mystruct->type = getInt(socket_fd);
	mystruct->error_id = getInt(socket_fd);
	//mystruct->path_length = getInt(socket_fd);
	//char *path = malloc(sizeof(char) * mystruct->path_length);
	getData(socket_fd, mystruct->path, mystruct->path_length);
	//mystruct->path = path;
	mystruct->offset = getInt(socket_fd);
	mystruct->count = getInt(socket_fd);
	mystruct->data_length = getInt(socket_fd);
	mystruct->data = malloc(sizeof(char) * mystruct->data_length);
	getData(socket_fd, mystruct->data, mystruct->data_length);
}

void sendStruct(int socket_fd, rm_protocol *data) {
	sendInt(socket_fd, data->pid);
	sendInt(socket_fd, data->filepart);
	sendInt(socket_fd, data->type);
	sendInt(socket_fd, data->error_id);
	//sendInt(socket_fd, data->path_length);
	sendData(socket_fd, data->path);
	sendInt(socket_fd, data->offset);
	sendInt(socket_fd, data->count);
	//sendInt(socket_fd, data->data_length);
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
	tosend->data = "";
	tosend->data_length = 0;
	tosend->error_id = 0;
	tosend->filepart = 0;
	tosend->offset = offset;
	tosend->path = path;
	tosend->path_length = strlen(path);
	tosend->pid = pid;
	tosend->type = MAP;
}

void makeReadRequest(rm_protocol *tosend, int pid, uint32_t filepartid, int offset, int count){
	tosend->count = count;
	tosend->data = "";
	tosend->data_length = 0;
	tosend->error_id = 0;
	tosend->filepart = filepartid;
	tosend->offset = offset;
	tosend->path = "";
	tosend->path_length = 0;
	tosend->pid = pid;
	tosend->type = READ;
}
void makeWriteRequest(rm_protocol *tosend, uint32_t fpart, int pid, char *data, int offset, int count){
	tosend->count = count;
	tosend->data = data;
	tosend->data_length = strlen(data);
	tosend->error_id = 0;
	tosend->filepart = fpart;
	tosend->offset = offset;
	tosend->path = "";
	tosend->path_length = 0;
	tosend->pid = pid;
	tosend->type = WRITE;
}
void makeUnmapRequest(rm_protocol *tosend, uint32_t fpart, int pid){
	tosend->count = 0;
	tosend->data = "";
	tosend->data_length = 0;
	tosend->error_id = 0;
	tosend->filepart = fpart;
	tosend->offset = 0;
	tosend->path = "";
	tosend->path_length = 0;
	tosend->pid = pid;
	tosend->type = UNMAP;
}

