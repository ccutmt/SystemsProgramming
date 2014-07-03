#ifndef COORDINATOR_H_
#define COORDINATOR_H_

#include <signal.h>
#include <string.h>
#include <time.h>
#include "message_queues.h"
#include "memsharing.h"
#include "semsharing.h"
#include "../ArrayList/ArrayList.h"

struct map_info{
	int mapid;				//Unique id given to user
	unsigned int fileid;	//File id given by server
	ArrayList *offsets;		//List of map_part_info
	int offset;				//Offset requested by user
	struct in_addr ip;
	int port;
};
typedef struct map_info map_info;

typedef struct map_part_info{
	unsigned long part_offset;		//Offset in shared memory
	unsigned int timestamp;			//Last read time

} map_part_info;

int _queue_id;
int is_master;
sigset_t sigusr1only, all;

//Stores mappings
ArrayList *addressmap;

//used to generate ids for mapping. These are returned to the user
int idcount;

int initCoordinator();
void destroyCoordinator();
void cleanUp(int signo);
int getFilePartOffset(unsigned long fileid, unsigned long serverip, unsigned long offset);
int getFirstFreePart();
void getUpdatedDataFromServer(int fileid, unsigned long offset, _shared_file *read, struct in_addr ip, int port, int off_mem);
void makeRequest(rm_protocol *tosend, rm_protocol *reply, struct in_addr ip, int port);

void requestServer(rm_protocol *tosend, rm_protocol *reply, int fd);
void incrementUsers(int offset);
void decrementUsers(int offset);
int makeMap(rm_protocol *reply, struct in_addr ip, unsigned long off);
int makeUnmap(int offset, struct in_addr ip, int port);
int makeRead(int fileid, unsigned long realoff, struct in_addr ip, int port);

int getMappingByAddr(void *addr);
int checkIfRead(map_info *mapping, int newmap);

void setSignalMaskUsr1Only();
void unblockAllSignal();

/*
 * Message queue methods
 */
void newMsgInQueue(int signo);
int sendMsg(rqst_over_queue *tosend);
void waitForSignal();



#endif /* COORDINATOR_H_ */
