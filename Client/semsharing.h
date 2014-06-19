#ifndef SEMSHARING_H_
#define SEMSHARING_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>

int getSemId(key_t key);
void removeSem(int id);
void updateSem(int id, int semnum, int amount);
int getSemVal(int id);

#endif /* SEMSHARING_H_ */
