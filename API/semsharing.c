#include "semsharing.h"

int getSemId(key_t key, int size){
	int id = -1;

	//Assume semaphore doesn't exist
	if((id=semget(key, size, IPC_CREAT  |  IPC_EXCL | 0666)) == -1){
		//If semaphore exists retry with different privileges
		if((id=semget(key, size, 0)) == -1){
			perror("Cannot create shared semaphore.");
			return -1;
		}
	}
	else{
		//Initialize semaphore
		short vals[] = {0,0};
		if (semctl(id, 0, SETALL, &vals) < 0) {
			perror("Could not set value of semaphore");
		}
	}

	return id;
}

int initSemaphores(){
	sem_header_set = sem_data_set = -1;
	sem_header_set = getSemId(SEM_HEADER_ID, 2);
	sem_data_set = getSemId(SEM_DATA_ID, 2);

	//Make sure semaphore sets got created
	if(sem_header_set == -1 || sem_data_set == -1){
		return -1;
	}

	return 0;
}

void removeSemaphores(){
	removeSem(sem_header_set);
	removeSem(sem_data_set);
}

void removeSem(int id){
	//Remove semaphore
	if(semctl(id, 0, IPC_RMID) != 0){
		perror("Error while removing semaphore");
	}
}

void updateSem(int id, struct sembuf* sops, int nsops){
	if (semop(id, sops, nsops) == -1) {
		perror("Could not update semaphore");
	}
}

void requestRead(int id){
	struct sembuf* sops = (struct sembuf*)malloc(sizeof(struct sembuf)*2);
	//Wait for writers to become 0
	sops[0].sem_num = 1;
	sops[0].sem_op = 0;
	sops[0].sem_flg = 0;
	//Request lock
	sops[1].sem_num = 0;
	sops[1].sem_op = 1;
	sops[1].sem_flg = 0;
	updateSem(id, sops, 2);
	free(sops);
}

void requestWrite(int id){
	struct sembuf* sops = (struct sembuf*)malloc(sizeof(struct sembuf)*3);
	//Wait for readers to become 0
	sops[0].sem_num = 0;
	sops[0].sem_op = 0;
	sops[0].sem_flg = 0;
	//Wait for writers to become 0
	sops[1].sem_num = 1;
	sops[1].sem_op = 0;
	sops[1].sem_flg = 0;
	//Request lock
	sops[2].sem_num = 1;
	sops[2].sem_op = 1;
	sops[2].sem_flg = 0;
	updateSem(id, sops, 3);
	free(sops);
}

void releaseRead(int id){
	struct sembuf* sops = (struct sembuf*)malloc(sizeof(struct sembuf));
	//Remove lock
	sops[0].sem_num = 0;
	sops[0].sem_op = -1;
	sops[0].sem_flg = 0;
	updateSem(id, sops, 1);
	free(sops);
}

void releaseWrite(int id){
	struct sembuf* sops = (struct sembuf*)malloc(sizeof(struct sembuf));
	//Remove lock
	sops[0].sem_num = 1;
	sops[0].sem_op = -1;
	sops[0].sem_flg = 0;
	updateSem(id, sops, 1);
	free(sops);
}
