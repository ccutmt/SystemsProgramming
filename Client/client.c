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
#include "net_protocol.h"

int main(int argc, char **argv) {
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_fd < 0)
		printf("Could not create socket");
	else
		printf("Socket Created\n");

	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	addr.sin_addr.s_addr = inet_addr("192.168.3.104");

	int res;
	if ((res = connect(socket_fd, (struct sockaddr *) &addr, sizeof(addr)))
			< 0) {
		printf("%s\n", strerror(res));
		return -1;
	} else
		printf("Connected to Server\n");

	//char *buf = malloc(sizeof(char) * 255);
	//*buf = "20Hi";
	// Send message to the server
	int test = 2;
	int32_t converted_number = htonl(test);
	int n = write(socket_fd, &converted_number, sizeof(converted_number));
	if (n < 0) {
		perror("ERROR writing to socket");
		exit(1);
	}

	int reply = 0;
	while (read(socket_fd, &reply, sizeof(reply)) > 0) {
		printf("%i", ntohl(reply));
	}
	if(ntohl(reply) == ERROR){
		printf("There was an error");
	}
	else if(ntohl(reply) == ACK){
		printf("acknowledged");
	}

	//free(buf);

	close(socket_fd);

	return 0;
}
