#include "coordinator.h"

int is_master = -1;
int socket_no = -1;

int initCoordinator(){
	//Create message queue and register signal
	if(signal(SIGUSR1, newMsgInQueue) == SIG_ERR)
		return -1;
	if(signal(SIGTERM, cleanUp) == SIG_ERR)
		return -1;
	if(signal(SIGINT, cleanUp) == SIG_ERR)
		return -1;
	if((_queue_id = getQueueId(_MSGQUEUE_KEY)) == -1)
		return -1;

	//Init semaphores
	if(initSemaphores() == -1)
		return -1;

	//Attach shared memory segments
	if((_header_mem_id = getShmId(_HEADER_KEY, sizeof(int))) == -1)
		return -1;
	if((_data_mem_id = getShmId(_DATA_KEY, sizeof(_shared_file) * 10)) == -1)
		return -1;
	if((_header_memory = attachKey(_header_mem_id)) == (void*) -1)
		return -1;
	if((_data_memory = attachKey(_data_mem_id)) == (void*) -1)
		return -1;

	//Check if master exists and add number of processes that are alive
	requestWrite(sem_header_set);

	int pno = getSharedInt(_header_memory + sizeof(int));
	int master = getSharedInt(_header_memory);

	if(master == 0){
		setSharedInt(_header_memory, getpid());
		is_master = 0;
	}
	setSharedInt(_header_memory + sizeof(int), pno+1);
	releaseWrite(sem_header_set);

	atexit(destroyCoordinator);

	return 0;
}

void decodeMsgMaster(){
	rqst_over_queue *msg = receiveMsgQueue(_queue_id);
	if(msg == NULL)
		perror("Message was null");
	else{
		rm_protocol *tosend = malloc(sizeof(rm_protocol));
		int request = msg->request;

		switch(request){
		case MAP: {

			break;
		}
		case UNMAP: {
			break;
		}
		case READ: {

			break;
		}
		case WRITE: {
			//Read file part from memory
			requestRead(sem_data_set);

			_shared_file *read = malloc(sizeof(_shared_file));
			readSharedData(msg->offset, read);

			releaseRead(sem_data_set);

			tosend->data_length = _DATA_LENGTH;
			tosend->data = read->data;
			tosend->filepart = read->fileid;
			tosend->pid = msg->owner;
			tosend->error_id = 0;
			tosend->count = 5;
			tosend->offset = 0;
			tosend->path = " ";
			tosend->path_length = 1;
			tosend->type = WRITE;
			break;
		}
		case ACK:
		case ERROR:
			break;
		}
		sendStruct(socket_no, tosend);
		free(tosend);
		tosend = readFromNet(socket_no);
		//actForServerReply(tosend);
		free(tosend);
		free(msg);
	}
}

void getUpdatedDataFromServer(int filepartid, int offset){

}

rqst_over_queue * actForServerReply(rm_protocol *reply, int prevRequest){
	int type = reply->type;
	rqst_over_queue *msg = malloc(sizeof(rqst_over_queue));
	switch(type){
	case ACK:{
		msg->error = 0;
		msg->offset = reply->filepart;
		msg->owner = reply->pid;
		msg->pid = reply->pid;
		msg->request = ACK;
		break;
	}
	case ERROR:
	case MAP:
	case READ:{
		if(prevRequest == WRITE){

		}
		//int firstfree = getFirstFreeBlock();
		requestWrite(sem_data_set);
		_shared_file *towrite = malloc(sizeof(_shared_file));
		memcpy(towrite->data, reply->data, _DATA_LENGTH);
		towrite->fileid = reply->filepart;
		towrite->pid = 0;
		towrite->pno = 1;
		towrite->serverip = 1234; //need to get ip for server
		free(towrite);
		releaseWrite(sem_data_set);
		msg->error = 0;
		//msg->offset = firstfree;
		msg->owner = reply->pid;
		//msg->path = "";
		msg->pid = reply->pid;
		msg->request = ACK;
		break;
	}

	}
	return msg;
}

void newMsgInQueue(int signo){
	//Signal handler to receive messages on queues
	if(is_master == 0){
		decodeMsgMaster();
	}
}

int sendMsg(rqst_over_queue* msgp){
	//Send message to process and send signal
	if(sendMsgQueue(_queue_id, msgp) == 0){
		kill(msgp->pid, SIGUSR1);
		return 0;
	}else {
		return -1;
	}
}

int decodeReplyProcess(){
	/*rqst_over_queue *msg = receiveMsgQueue(_queue_id);
	if(msg == NULL){
		perror("Message was null");
	}
	else{
		int type = msg->request;
		switch(type){
		case ACK: {
			return msg->offset;
		}
		case ERROR:{
			return -1;
		}
		}
	}*/
	return -1;
}

int getFilePartOffset(unsigned long fileid, unsigned long serverip){
	void * idloc = _data_memory;
	int count = 0;
	requestRead(sem_data_set);
	while(count < 10){
		if(*(unsigned long*)(idloc + sizeof(int) * 2) == fileid){
			if(*(unsigned long*)(idloc + (sizeof(int) * 2) + sizeof(unsigned long)) == serverip){
				releaseRead(sem_data_set);
				return count;
			}
		}
		idloc += sizeof(_shared_file);
		count++;
	}
	releaseRead(sem_data_set);
	return -1;
}

void destroyCoordinator(){
	//Consume any messages remaining on queue
	rqst_over_queue *temp = NULL;
	while((temp = receiveMsgQueue(_queue_id)) != NULL){
		free(temp);
	}

	//If this is the last process alive, destroy IPC structures
	requestWrite(sem_header_set);
	int pno = getSharedInt(_header_memory + sizeof(int));
	setSharedInt(_header_memory + sizeof(int), pno - 1);
	if(is_master == 0)
		setSharedInt(_header_memory, 0);

	detachKey(_header_memory);
	detachKey(_data_memory);
	if(pno == 1){
		removeQueue(_queue_id);
		removeMemSeg(_header_mem_id);
		removeMemSeg(_data_mem_id);
		removeSemaphores();
	}
	else{
		releaseWrite(sem_header_set);
	}
}

/*
 * Function used to catch SIGINT interrupt so that the program terminates normally
 */
void cleanUp(int signo){
	exit(0);
}

int getSocket(){
	return socket_no;
}

void setSocket(int sock){
	socket_no = sock;
}

void requestServer(rm_protocol *tosend, rm_protocol *reply, int fd){
	sendStruct(fd, tosend);
	readFromNet(fd, reply);
}
