#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "net_protocol.h"
#include "data_structure.h"

int maxfd;

int acceptConnection(int socket, fd_set *listening);
void manager(rm_protocol *received, rm_protocol *reply, int fd);
void populateSet(fd_set *listening);

#endif // TCP_SERVER_H
