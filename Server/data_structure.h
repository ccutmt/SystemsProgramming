#ifndef DATA_STRUCTURE_H_
#define DATA_STRUCTURE_H_

#include "connections.h"
#include "files.h"
#include "ArrayList.h"
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
}rmfile;

ArrayList * files;

int isValid(int fd, filepart * part);
filepart * getFilePart(off_t start_offset, rmfile * file);
rmfile * getFile(char *path);
void setLastWriter(int user_fd, filepart *part);
int getUserInfoOffset(connection *rem, filepart *fp);
int getFilePartOffset(filepart * rem, rmfile *file);
int getRmfileOffset(rmfile *rem);
void unmapRequest(char * path, off_t offset, connection *conn);
void mapRequest(char * path, off_t offset, connection *conn, char *buff);
void readRequest(char * path, off_t offset, size_t length, char * buff, connection *conn);

#endif /* DATA_STRUCTURE_H_ */