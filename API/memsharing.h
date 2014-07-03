#ifndef MEMSHARING_H_
#define MEMSHARING_H_

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>

#define _HEADER_KEY 252458
#define _DATA_KEY 252459
#define _DATA_LENGTH 256

struct _shared_file{
	unsigned int write_timestamp;
	unsigned int pno;
	unsigned long fileid;
	unsigned long serverip;
	unsigned long offset;
	char data[_DATA_LENGTH];
};
typedef struct _shared_file _shared_file;

int _header_mem_id;
int _data_mem_id;
void *_header_memory;
void *_data_memory;

int getShmId(key_t key, size_t size);
void* attachKey(int id);
void detachKey(void* addr);
void removeMemSeg(int id);
int getSharedInt(void* pos);
void setSharedInt(void* pos, int pid);
void writeSharedData(_shared_file* towrite, int offset);
void readSharedData(int offset, _shared_file *result);

#endif /* MEMSHARING_H_ */
