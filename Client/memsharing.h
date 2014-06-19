#ifndef MEMSHARING_H_
#define MEMSHARING_H_

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define MASTER_KEY 252458

int getMaster(void* pos);
void setMaster(void* pos, int pid);
void* attachKey(key_t key, size_t size);
void detachKey(void* addr);
void removeMemSeg(key_t key, size_t size);


#endif /* MEMSHARING_H_ */
