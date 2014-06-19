#include "memsharing.h"

int getMaster(void* pos){
	return *(int*) pos;
}

void setMaster(void* pos, int id){
	*(int*)pos = id;
}

void* attachKey(key_t key, size_t size){
	void* keyloc = (void*)-1;
	int id = -1;

	//Assume shared memory segment doesn't exist
	if((id=shmget(key, size, IPC_CREAT  |  IPC_EXCL | 0666)) == -1){
		//If segment exists retry with different privileges
		if((id=shmget(key, size, 0)) == -1){
			perror("Cannot create shared segment.");
			return keyloc;
		}
	}

	//Attach memory segment
	if ((keyloc = (void*)shmat(id, NULL, 0)) == (void*) -1) {
		perror("Error while attaching shared segment.");
	}

	return keyloc;
}

void detachKey(void* addr){
	int res = -1;
	if((res = shmdt(addr)) != 0){
		perror("Error while detaching");
	}
}

void removeMemSeg(key_t key, size_t size){
	int id = -1;

	if((id=shmget(key, size, 0)) == -1){
		perror("Cannot get segment id");
		return;
	}

	//Remove shared memory segment
	if(shmctl(id, IPC_RMID, NULL) != 0){
		perror("Error while removing shared memory");
	}
}
