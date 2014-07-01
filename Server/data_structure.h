#ifndef DATA_STRUCTURE_H_
#define DATA_STRUCTURE_H_

#include "connections.h"
#include "files.h"
#include "ArrayList.h"
#include "net_protocol.h"
#include <string.h>

#define _DATA_LENGTH 256

typedef struct user_info{
	connection * conn;
	int isvalid;
} user_info;

typedef struct filepart{
	ArrayList *entries;
	off_t start_offset;
} filepart;

typedef struct rmfile
{
    char * pathname;
    int fd;
    ArrayList * fileparts;
    unsigned long fileid;
} rmfile;

void initDataStructure();
filepart * getFilePart(off_t start_offset, rmfile * file);
rmfile * getFile(char *path);
rmfile * getFileById(unsigned long id);
void setLastWriter(int user_fd, filepart *part);
int getUserInfoOffset(connection *rem, filepart *fp);
int getFilePartOffset(filepart * rem, rmfile *file);
int getRmfileOffset(rmfile *rem);
void unmapRequest(unsigned long id, off_t offset, connection *conn, int *err);
void mapRequest(char *path, off_t offset, connection *conn, char *buff, unsigned long *fid, int *err);
void readRequest(unsigned long id, off_t offset, size_t length, rm_protocol *reply, connection *conn);
void writeRequest(unsigned long id, off_t offset, size_t length, char *buff, rm_protocol *reply, connection *conn);

#endif /* DATA_STRUCTURE_H_ */
