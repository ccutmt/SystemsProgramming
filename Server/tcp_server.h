#ifndef TCP_SERVER_H
#define TCP_SERVER_H

typedef struct connections
{
    int id;
    unsigned long ip;	
}connections;

int acceptConnection(int socket, fd_set *listening);

#endif // TCP_SERVER_H
