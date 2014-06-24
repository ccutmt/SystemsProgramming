#ifndef MESSAGE_QUEUES_H_
#define MESSAGE_QUEUES_H_

#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "net_protocol.h"

struct rqst_over_queue{
	long pid;
	rm_protocol message;
	struct in_addr ipaddress;
	int port;
	/*int owner;
	int request;
	int offset;
	int error;
	char path[256];*/
};
typedef struct rqst_over_queue rqst_over_queue;

#define _MSGQUEUE_KEY 525312

int getQueueId(key_t key);
void removeQueue(int id);
int sendMsgQueue(int qid, rqst_over_queue *tosend);
int receiveMsgQueue(int id, rqst_over_queue*);

#endif /* MESSAGE_QUEUES_H_ */
