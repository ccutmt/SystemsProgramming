#ifndef FILES_H
#define FILES_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int openFile(char* pathname);
int closeFile(int fd);
int readFile(char* buff, size_t length, int fd, off_t offset);

#endif 
