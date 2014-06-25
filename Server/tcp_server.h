#ifndef TCP_SERVER_H
#define TCP_SERVER_H

typedef struct connections
{
    int id;
    unsigned long ip;	
}connections;

typedef struct file
{
    int fid;
    char * pathname;
    FILE * fd;
}file;

void openFile(char f[]);
void closeFile(FILE * f);
#endif // TCP_SERVER_H
