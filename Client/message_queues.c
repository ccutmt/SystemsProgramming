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

void sendMsgQueue(int qid, int dest_pid, int offset, int request, int error){
	rqst_over_queue *msgp = (rqst_over_queue*) malloc(sizeof(rqst_over_queue));
	msgp->pid = dest_pid;
	msgp->offset = offset;
	msgp->error = error;
	msgp->request = request;
	msgsnd(qid, msgp, sizeof(rqst_over_queue), IPC_NOWAIT);
	free(msgp);
}

rqst_over_queue* receiveMsgQueue(int qid){
	rqst_over_queue *msgp = (rqst_over_queue*) malloc(sizeof(rqst_over_queue));
	if(msgrcv(qid, msgp, sizeof(rqst_over_queue), getpid(), IPC_NOWAIT) == -1){
		free(msgp);
		return NULL;
	}
	return msgp;
}
