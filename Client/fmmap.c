#include "fmmap.h"

#include <sys/ipc.h>
#include <sys/shm.h>

#define MAP_FAILED -1
#define MASTER_KEY 252458

int shmid = -1;

void * rmmap(fileloc_t location, off_t offset) {

	/*if (shared_struct_key == -1) {
		if ((shared_struct_key = ftok("/etc", 85744)) == (key_t)-1) {
			errno = MAP_FAILED;
			perror("Error generating shared memory id");
			return (void*) -1;
		} else
			printf("Ftok successful");
	} else
		printf("found key!\n");

	shmid = shmget(shared_struct_key, sizeof(rm_list), IPC_CREAT  |  IPC_EXCL | 0666);
	if (shmid == -1){
		shmid = shmget(shared_struct_key, sizeof(rm_list), 0);
		if(shmid == -1){
			perror("Cannot create shared memory!");
			errno = -1;
			return (void*)-1;
		}
	}
	else
		printf("res is: %u\n", shmid);

	if ((shared_struct = shmat(shmid, NULL, 0)) != NULL) {
		//Check if there already exists a table
		if(shared_struct->head == NULL){
			int id = -1;
			//Create connection entry
			if((id=shmget(shared_struct_key+1, sizeof(connection_t), IPC_CREAT  |  IPC_EXCL | 0666)) != -1){
				//Attach connection to program memory
				if ((shared_struct->head = (void*)shmat(id, NULL, 0)) != NULL) {
					shared_struct->size = 1;
					connection_t * con = (connection_t*)shared_struct->head;
					con->next = NULL;
					con->file_descriptor = 1000;
				}else{
					perror("head is null");
				}
			}else{
				errno = -1;
				perror("Creating connection");
			}
		}
		else {
			printf("Table exists");
			int id = -1;
			if((id=shmget(shared_struct_key+1, sizeof(connection_t), 0)) != -1){
				if ((shared_struct->head = (void*)shmat(id, NULL, 0)) != NULL) {
					connection_t * temp = shared_struct->head;
					printf("\n%i\n", temp->file_descriptor);
				}
			}

		}*/
		/*if(*shared_struct != 'a')
			*shared_struct = 'a';
		else printf("found a!");*/
	/*} else
		perror("error while attaching");
	 */
	return (void*) -1;
}

int rmunmap(void *addr){
	/*if(shared_struct == NULL){
		errno = -1;
		return -1;
	}

	if(shared_struct->size == 0){
		errno = -1;
		return -1;
	}
	if(shared_struct->size == 1){
		if(!shmdt(shared_struct))
			perror("Error while detaching shared memory");

		//Remove shared memory segment
		if(!shmctl(shmid, IPC_RMID, NULL)){
			perror("Error while removing shared memory");
		}
	}*/
	return 0;
}
