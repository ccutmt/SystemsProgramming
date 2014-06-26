#ifndef COORDINATOR_H_
#define COORDINATOR_H_

#include <signal.h>
#include <string.h>
#include <time.h>
#include "message_queues.h"
#include "memsharing.h"
#include "semsharing.h"

int _queue_id;
int is_master;

int initCoordinator();
void destroyCoordinator();
void cleanUp(int signo);
int getFilePartOffset(unsigned long fileid, unsigned long serverip, unsigned long offset);
int getFirstFreePart();
void getUpdatedDataFromServer(int filepartid, int offset);

void requestServer(rm_protocol *tosend, rm_protocol *reply, int fd);
void incrementUsers(int offset);
void decrementUsers(int offset);
int makeMap(rm_protocol *reply, struct in_addr ip, unsigned long off);
int makeUnmap(int offset, struct in_addr ip, int port);
int makeRead(int fileid, int realoff, struct in_addr ip, int port);
int makeWrite(int fileid, struct in_addr ip, int port);

/*
 * Message queue methods
 */
void newMsgInQueue(int signo);
int sendMsg(rqst_over_queue *tosend);
void waitForSignal();



#endif /* COORDINATOR_H_ */
