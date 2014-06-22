#ifndef MESSAGE_QUEUES_H_
#define MESSAGE_QUEUES_H_

#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct rqst_over_queue{
	long pid;
	int offset;
	int request;
	int error;
};
typedef struct rqst_over_queue rqst_over_queue;

#define _MSGQUEUE_KEY 525312

rqst_over_queue* reply_queue;

int getQueueId(key_t key);
void removeQueue(int id);
int sendMsgQueue(int qid, int dest_pid, int offset, int request, int error);
rqst_over_queue* receiveMsgQueue(int id);

#endif /* MESSAGE_QUEUES_H_ */
