#include "message_queues.h"

int getQueueId(key_t key, int size){
	int id = -1;

	//Assume queue doesn't exist
	if((id=msgget(key, IPC_CREAT  |  IPC_EXCL | 0666)) == -1){
		//If queue exists retry with different privileges
		if((id=msgget(key, 0)) == -1){
			perror("Cannot create shared semaphore.");
			return -1;
		}
	}
	return id;
}

void removeQueue(int id){
	//Remove queue
	if(msgctl(id, 0, IPC_RMID) != 0){
		perror("Error while removing semaphore");
	}
}

void sendMsgQueue(int id){
	struct msgbuf *msgp = (struct msgbuf*)malloc(sizeof(struct msgbuf));
	msgsnd(id, msgp, );
}
