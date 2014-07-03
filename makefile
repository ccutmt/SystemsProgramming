CC=gcc
CFLAGS=-Wall

fmserver: ArrayList/ArrayList.c Protocol/net_protocol.c Server/connections.c Server/data_structure.c Server/files.c Server/fmserver.c 
	$(CC) -o fmserver ArrayList/ArrayList.c Protocol/net_protocol.c Server/connections.c Server/data_structure.c Server/files.c Server/fmserver.c -Wall

client: ArrayList/ArrayList.c Protocol/net_protocol.c API/coordinator.c API/fmmap.c API/memsharing.c API/message_queues.c API/semsharing.c Client/client.c
	$(CC) -o client ArrayList/ArrayList.c Protocol/net_protocol.c API/coordinator.c API/fmmap.c API/memsharing.c API/message_queues.c API/semsharing.c Client/client.c -Wall
