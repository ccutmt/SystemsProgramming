#ifndef COORDINATOR_H_
#define COORDINATOR_H_

#include <signal.h>
#include "message_queues.h"
#include "memsharing.h"
#include "semsharing.h"

int _queue_id;
int _header_mem_id;
int _data_mem_id;
void *_header_memory;
void *_data_memory;

int initCoordinator();
void newMsgInQueue(int signo);
int sendMsg(int dest_pid, int offset, int request, int error);
int getSharedInt(void* pos);
void setSharedInt(void* pos, int pid);
void destroyCoordinator();

void cleanUp(int signo);


#endif /* COORDINATOR_H_ */
