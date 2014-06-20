#ifndef MESSAGE_QUEUES_H_
#define MESSAGE_QUEUES_H_

#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>

#define MSGQUEUE_KEY 525312

int getQueueId(key_t key, int size);
void removeQueue(int id);
void sendMsgQueue(int id);
void receiveMsgQueue(int id);

#endif /* MESSAGE_QUEUES_H_ */
