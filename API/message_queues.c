#include "message_queues.h"

int getQueueId(key_t key){
	int id = -1;

	//Assume queue doesn't exist
	if((id=msgget(key, IPC_CREAT  |  IPC_EXCL | 0666)) == -1){
		//If queue exists retry with different privileges
		if((id=msgget(key, 0)) == -1){
			perror("Cannot create shared queue.");
			return -1;
		}
	}
	return id;
}

void removeQueue(int id){
	//Remove queue
	if(msgctl(id, 0, IPC_RMID) != 0){
		perror("Error while removing queue");
	}
}

int sendMsgQueue(int qid, rqst_over_queue *tosend){
	int result = -1;
	result = msgsnd(qid, tosend, sizeof(rqst_over_queue), IPC_NOWAIT);
	return result;
}

int receiveMsgQueue(int qid, rqst_over_queue* msgp){
	if(msgrcv(qid, msgp, sizeof(rqst_over_queue), getpid(), IPC_NOWAIT) == -1){
		return -1;
	}
	return 0;
}
