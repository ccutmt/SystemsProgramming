#ifndef COORDINATOR_H_
#define COORDINATOR_H_

#include <signal.h>
#include <string.h>
#include "message_queues.h"
#include "memsharing.h"
#include "semsharing.h"

int _queue_id;
int is_master;

int initCoordinator();
void destroyCoordinator();
void cleanUp(int signo);
int getFilePartOffset(unsigned long fileid, unsigned long serverip);
int getFirstFreePart();
void getUpdatedDataFromServer(int filepartid, int offset);

void requestServer(rm_protocol *tosend, rm_protocol *reply, int fd);
int makeMap(rm_protocol *reply, struct in_addr ip);

/*
 * Message queue methods
 */
void newMsgInQueue(int signo);
int sendMsg(rqst_over_queue *tosend);



#endif /* COORDINATOR_H_ */
