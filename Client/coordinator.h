#ifndef COORDINATOR_H_
#define COORDINATOR_H_

#include <signal.h>
#include <string.h>
#include "net_protocol.h"
#include "message_queues.h"
#include "memsharing.h"
#include "semsharing.h"
#include "semsharing.h"

int _queue_id;

int initCoordinator();
void destroyCoordinator();
void cleanUp(int signo);
int getFilePartOffset(unsigned long fileid, unsigned long serverip);
void decodeMsgMaster();
rqst_over_queue * actForServerReply(rm_protocol *tosend, int prevRequest);
void getUpdatedDataFromServer(int filepartid, int offset);
int decodeReplyProcess();
int getSocket();
void setSocket(int sock);

void requestServer(rm_protocol *tosend, rm_protocol *reply, int fd);
void makeMap(rm_protocol *reply);

/*
 * Message queue methods
 */
void newMsgInQueue(int signo);
int sendMsg(rqst_over_queue *tosend);



#endif /* COORDINATOR_H_ */
