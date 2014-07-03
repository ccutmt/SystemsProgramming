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
#include "../API/fmmap.h"

int main(int argc, char **argv) {

	/*
	 * test library
	 */
	struct in_addr ip;
	ip.s_addr = inet_addr("127.0.0.1");
	fileloc_t fl;
	fl.ipaddress = ip;
	fl.pathname = "Server/FILE_2.txt";
	fl.port = 8080;
	//printf("pid: %i", getpid());
	printf("Press Enter to map.\n");
	void *point = rmmap(fl, 0);
	perror("map");
	//printf("%i", *(int*)point);
	printf("Press Enter to read.\n");
	getchar();
	char *buff = malloc(sizeof(char) * 5);
	printf("Read: %zu", mread(point, 0, buff, 5));
	perror("read");
	printf("Press Enter to write.\n");
	getchar();
	*(buff) = 'Z';
	//printf("Data is: %s", buff);
	printf("written: %zu", mwrite(point, 0, buff, 5));
	perror("write");
	printf("Press Enter to unmap.\n");
	getchar();
	rmunmap(point);
	perror("Unamp");
	getchar();
	return 0;
}
