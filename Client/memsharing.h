#ifndef MEMSHARING_H_
#define MEMSHARING_H_

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define _HEADER_KEY 252458
#define _DATA_KEY 252459

int getShmId(key_t key, size_t size);
void* attachKey(int id);
void detachKey(void* addr);
void removeMemSeg(int id);


#endif /* MEMSHARING_H_ */
