#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "net_protocol.h"

int maxfd;

int acceptConnection(int socket, fd_set *listening);

#endif // TCP_SERVER_H
