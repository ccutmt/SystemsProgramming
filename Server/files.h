#ifndef FILES_H
#define FILES_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int openFile(char* pathname, int *err);
int closeFile(int fd, int *err);
int readFile(char* buff, size_t length, int fd, off_t offset, int *err);
int writeFile(char *buff, size_t length, int fd, off_t offset, int *err);

#endif 
