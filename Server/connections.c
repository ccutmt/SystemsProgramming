#include "connections.h"

void addConnection(int fd, unsigned long ip){
	connection *search = getConnectionByIp(ip);
	if(search != NULL){
		search = malloc(sizeof(connection));
		search->fd = fd;
		search->ip = ip;
		add(connections, search);
	}else{
		search->fd = fd;
	}
}

connection * getConnectionByIp(unsigned long ip){
	int c = 0;
	connection * result = NULL;
	while(c <= connections->current){
		connection * temp = getElement(connections, c);
		if(temp->ip == ip){
			return temp;
		}
		c++;
	}
	return result;
}

int getConnectionOffset(connection *con){
	int c = 0;
	while(c <= connections->current){
		connection * temp = getElement(connections, c);
		if(temp == con){
			return c;
		}
		c++;
	}
	return -1;
}
