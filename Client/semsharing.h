#ifndef SEMSHARING_H_
#define SEMSHARING_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>

#define SEM_HEADER_ID 65525
#define SEM_DATA_ID 65526

int initSemaphores();
void removeSemaphores();
int getSemId(key_t key, int size);
void removeSem(int id);
void updateSem(int id, struct sembuf* sops ,int snum);
int getSemVal(int id, int pos);
void requestRead(int id);
void requestWrite(int id);
void removeRead(int id);
void removeWrite(int id);

int sem_header_set;
int sem_data_set;

#endif /* SEMSHARING_H_ */
