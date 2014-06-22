#include "coordinator.h"

int initCoordinator(){
	//Create message queue and register signal
	if(signal(SIGUSR1, newMsgInQueue) == SIG_ERR)
		return -1;
	if((_queue_id = getQueueId(_MSGQUEUE_KEY)) == -1)
		return -1;

	//Attach shared memory segments
	if((_header_mem_id = getShmId(_HEADER_KEY, sizeof(int)*2)) == -1)
		return -1;
	if((_data_mem_id = getShmId(_DATA_KEY, sizeof(int))) == -1)
		return -1;
	if((_header_memory = attachKey(_header_mem_id)) == (void*) -1)
		return -1;
	if((_data_memory = attachKey(_data_mem_id)) == (void*) -1)
		return -1;

	//Check if master exists and add number of processes that are alive
	if(getSharedInt(_header_memory) == 0){
		setSharedInt(_header_memory, getpid());
	}
	int pno = getSharedInt(_header_memory + sizeof(int)*2);
	setSharedInt(_header_memory + sizeof(int)*2, pno+1);

	atexit(destroyCoordinator);

	return 0;
}

void newMsgInQueue(int signo){
	//Signal handler to receive messages on queues
	reply_queue = receiveMsgQueue(_queue_id);
}

int sendMsg(int dest_pid, int offset, int request, int error){
	//Send message to process and send signal
	if(sendMsgQueue(_queue_id, dest_pid, offset, request, error) == 0){
		kill(dest_pid, SIGUSR1);
		return 0;
	}else return -1;
}

int getSharedInt(void* pos){
	return *(int*) pos;
}

void setSharedInt(void* pos, int id){
	*(int*)pos = id;
}

void destroyCoordinator(){
	int pno = getSharedInt(_header_memory + sizeof(int)*2);
	setSharedInt(_header_memory + sizeof(int)*2, pno - 1);
	detachKey(_header_memory);
	detachKey(_data_memory);

	//Consume any messages remaining on queue
	rqst_over_queue *temp = NULL;
	while((temp = receiveMsgQueue(_queue_id)) != NULL){
		free(temp);
	}

	//If this is the last process alive, destroy IPC structures
	if(pno == 1){
		removeQueue(_queue_id);
		removeMemSeg(_header_mem_id);
		removeMemSeg(_data_mem_id);
	}
}
