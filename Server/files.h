#ifndef FILES_H
#define FILES_H

typedef struct file
{
    char * pathname;
    int fd;
}file;

int openFile(char* pathname);
int closeFile(int fd);

#endif 
