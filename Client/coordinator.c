#include "coordinator.h"

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

	is_master = -1;
	if(master == 0){
		setSharedInt(_header_memory, getpid());
		is_master = 0;
	}
	setSharedInt(_header_memory + sizeof(int), pno+1);
	releaseWrite(sem_header_set);

	atexit(destroyCoordinator);

	return 0;
}

void getUpdatedDataFromServer(int filepartid, int offset){

}


void newMsgInQueue(int signo){
	//Signal handler to receive messages on queues
	//Needs to add functionality to cater for signals that where blocked
	sigset_t new, old;
	sigemptyset (&new);
	sigprocmask(SIG_BLOCK, &new, &old);
	if(is_master == 0){
		rqst_over_queue msg;
		rqst_over_queue reply_queue;

		receiveMsgQueue(_queue_id, &msg);

		rm_protocol tosend = msg.message;
		rm_protocol reply;

		int fd = getServerFd(msg.ipaddress, msg.port);
		requestServer(&tosend, &reply, fd);

		reply_queue.message = reply;
		reply_queue.pid = reply.pid;
		sendMsg(&reply_queue);
	}
	sigprocmask(SIG_BLOCK, &old, NULL);
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

int getFilePartOffset(unsigned long fileid, unsigned long serverip){
	void * idloc = _data_memory;
	int count = 0;
	while(count < 10){
		if(*(unsigned long*)(idloc + sizeof(unsigned int) * 2) == fileid){
			if(*(unsigned long*)(idloc + (sizeof(unsigned int) * 2) + sizeof(unsigned long)) == serverip){
				return count;
			}
		}
		idloc += sizeof(_shared_file);
		count++;
	}
	return -1;
}

int getFirstFreePart(){
	int count = 0;
	void * idloc = _data_memory;
	while(count < 10){
		if(*(int*)(idloc + sizeof(int)) == 0){
			return count;
		}
		count++;
	}
	return -1;
}

void destroyCoordinator(){
	//Consume any messages remaining on queue
	rqst_over_queue temp;
	while((receiveMsgQueue(_queue_id, &temp)) != -1){
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

void waitForSignal(){
	sigset_t mask;
	sigfillset(&mask);
	sigdelset(&mask, SIGUSR1);
	sigsuspend(&mask);
}

void incrementUsers(int offset){
	int users = getSharedInt(_data_memory + sizeof(_shared_file) * offset + sizeof(unsigned int));
	setSharedInt(_data_memory + sizeof(_shared_file) * offset + sizeof(unsigned int), users+1);
}

void decrementUsers(int offset){
	int users = getSharedInt(_data_memory + sizeof(_shared_file) * offset + sizeof(unsigned int));
	setSharedInt(_data_memory + sizeof(_shared_file) * offset + sizeof(unsigned int), users-1);
}

void requestServer(rm_protocol *tosend, rm_protocol *reply, int fd){
	sendStruct(fd, tosend);
	readFromNet(fd, reply);
}

int makeMap(rm_protocol *reply, struct in_addr ip){
	int firstfree = getFirstFreePart();
	if(firstfree != -1){
		_shared_file towrite;
		memcpy(&towrite.data, reply->data, _DATA_LENGTH);
		towrite.fileid = reply->filepart;
		towrite.write_timestamp = time(NULL);
		towrite.pno = 1;
		towrite.serverip = ip.s_addr;
		writeSharedData(&towrite, firstfree);
	}
	return firstfree;
}

/*
 * return of 0 means unmap failed
 * return of 1 means successful
 */
int makeUnmap(int offset, struct in_addr ip, int port){
	_shared_file *tounmap = malloc(sizeof(_shared_file));
	readSharedData(offset, tounmap);
	int users = tounmap->pno;
	if(users > 1){
		decrementUsers(offset);
		return 1;
	}else{
		rm_protocol tosend, reply;
		if(is_master == -1){
			rqst_over_queue *tosend_queue = malloc(sizeof(rqst_over_queue));
			rqst_over_queue *reply_queue = malloc(sizeof(rqst_over_queue));
			makeUnmapRequest(&tosend, tounmap->fileid, getpid());

			requestRead(sem_header_set);
			int read = getSharedInt(_header_memory);
			tosend_queue->pid = (long)read;
			releaseRead(sem_header_set);

			tosend_queue->message = tosend;
			tosend_queue->ipaddress = ip;
			tosend_queue->port = port;
			sendMsg(tosend_queue);
			free(tosend_queue);

			waitForSignal();

			if(receiveMsgQueue(_queue_id, reply_queue) == 0)
				memcpy(&reply, &reply_queue->message, sizeof(rm_protocol));
			else perror("Message not received");

			//free(reply_queue);

		}
		else{
			int sfd = getServerFd(ip, port);
			requestServer(&tosend, &reply, sfd);
		}

		//clear memory part
		bzero(_data_memory + (sizeof(_shared_file)*offset), sizeof(_shared_file));

		if(reply.type == ERROR)
			return 0;
		else return 1;
	}
}
