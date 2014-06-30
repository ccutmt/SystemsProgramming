#ifndef CONNECTIONS_H_
#define CONNECTIONS_H_

#include "ArrayList.h"
#include <stdlib.h>

typedef struct connection
{
    int fd;
    unsigned long ip;
}connection;

ArrayList *connections;

void addConnection(int fd, unsigned long ip);
connection * getConnectionByIp(unsigned long ip);
int getConnectionOffset(connection * c);

#endif /* CONNECTIONS_H_ */
