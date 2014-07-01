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
#include "fmmap.h"

int main(int argc, char **argv) {

	/*
	 * Socket creation and connection to server
	 */
	/*int result = initCoordinator();
		if(result == -1)
			printf("error");

	int master = getSharedInt(_header_memory);
	if(master == getpid()){
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_fd < 0)
		perror("Could not create socket");

	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int res;
	if ((res = connect(socket_fd, (struct sockaddr *) &addr, sizeof(addr)))
			< 0) {
		printf("%s\n", strerror(res));
		return -1;
	} else
		printf("Connected to Server\n");
	setSocket(socket_fd);
	}*/

	/*
	 * Sending of one struct
	 */
	/*rm_protocol *test = (rm_protocol*) malloc(sizeof(rm_protocol));
	test->type = 1;
	test->data = 0;
	test->data_length = 0;
	test->count = 0;
	test->path = 0;
	test->error_id = 0;
	test->offset = 50;
	sendStruct(socket_fd, test);

	close(socket_fd);*/

	/*
	 * Test master key
	 */
	/*void* t = attachKey(MASTER_KEY, sizeof(int));

	int id = getQueueId(MSGQUEUE_KEY);

	int m = getMaster(t);
	printf("%i", m);
	if(m == 0)
		setMaster(t, getpid());
	else{
		sendMsgQueue(id, getMaster(t), 1, 2, 0);
		printf("sent message to %i", getMaster(t));
	}
	getchar();

	rqst_over_queue *reply = receiveMsgQueue(id);
	if(reply != NULL){
		printf("%i\n", reply->request);
	}else printf("reply was empty\n");
	free(reply);
	detachKey(t);
	removeMemSeg(MASTER_KEY, sizeof(int));*/

	/*
	 * Test semaphore
	 */
	/*if(initSemaphores() ==
			if(master != getpid()){0){
		requestWrite(sem_header_set);
		printf("requested write");
		getchar();
		removeWrite(sem_header_set);
	}
	removeSemaphores();*/

	/*
	 * Test Queues
	 */
	/*int id = getQueueId(_MSGQUEUE_KEY);
	rqst_over_queue *send = malloc(sizeof(rqst_over_queue));
	send->error = 25;
	send->offset = 2;
	send->pid = getpid();
	send->request = 0;
	sendMsgQueue(id, send);
	rqst_over_queue *reply = receiveMsgQueue(id);
	printf("%i", reply->error);
	getchar();
	removeQueue(id);*/

	/*
	 * Test coordinator
	 */




	/*_shared_file* a = malloc(sizeof(_shared_file));
	a->data[0] = 'a';
	a->data[1] = 'b';
	a->data[2] = '\0';
	a->fileid = 1;
	a->pid = getpid();
	a->pno = 1;
	a->serverip = 123456;
	writeSharedData(a,5);
	free(a);

	requestRead(sem_header_set);
	master = getSharedInt(_header_memory);

	if(master != getpid()){
		sendMsg(master, 5 , WRITE, 0);
		sigset_t myset;
		sigsuspend(&myset);
		printf("master is: %i pid: %i", master, getpid());
		printf("reply was: %i", decodeReplyProcess());
	}
	releaseRead(sem_header_set);

	getchar();


	if(getSocket() != -1)
		close(getSocket());*/

	/*
	 * test library
	 */
	struct in_addr ip;
	ip.s_addr = inet_addr("127.0.0.1");
	fileloc_t fl;
	fl.ipaddress = ip;
	fl.pathname = "/home/matthew/Desktop/Sys/Server/FILE_2.txt";
	fl.port = 8080;
	printf("pid: %i", getpid());
	void *point = rmmap(fl, 0);
	//printf("%i", *(int*)point);
	getchar();
	char *buff = malloc(sizeof(char) * 20);
	printf("reading");
	printf("Read: %i", mread(point, 0, buff, 20));
	getchar();
	*(buff) = 'Z';
	printf("Data is: %s", buff);
	printf("written: %i", mwrite(point, 0, buff, strlen(buff)));

	printf("before unmap");
	getchar();
	rmunmap(point);
	getchar();

	return 0;
}
