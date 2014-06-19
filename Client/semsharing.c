#include "semsharing.h"

int getSemId(key_t key){
	int id = -1;

	//Assume semaphore doesn't exist
	if((id=semget(key, 1, IPC_CREAT  |  IPC_EXCL | 0666)) == -1){
		//If semaphore exists retry with different privileges
		if((id=semget(key, 1, 0)) == -1){
			perror("Cannot create shared semaphore.");
			return -1;
		}
	}
	else{
		//Initialize semaphore
		if (semctl(id, 0, SETVAL, 0) < 0) {
			perror("Could not set value of semaphore");
		}
	}

	return id;
}

void removeSem(int id){
	//Remove semaphore
	if(semctl(id, 0, IPC_RMID) != 0){
		perror("Error while removing semaphore");
	}
}

void updateSem(int id, int semnum, int amount){
	struct sembuf sop;
	sop.sem_num = 0;
	sop.sem_op = amount;
	sop.sem_flg = SEM_UNDO;
	if (semop(id, &sop, semnum) == -1) {
		perror("Could not update semaphore");
	}
}

int getSemVal(int id){
	int val = -1;
	if((val = semctl(id, 0, GETVAL, 0)) == -1){
		perror("Error while getting semaphore value");
	}
	return val;
}
