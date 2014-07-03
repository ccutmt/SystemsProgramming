C = gcc
CFLAGS = -Wall

HEADERS = ArrayList/ArrayList.h Protocol/net_protocol.h Server/connections.h Server/data_structure.h Server/files.h Server/fmserver.h 
OBJECTS = ArrayList/ArrayList.o Protocol/net_protocol.o Server/connections.o Server/data_structure.o Server/files.o Server/fmserver.o

HEADERS2 = ArrayList/ArrayList.h Protocol/net_protocol.h API/coordinator.h API/fmmap.h API/memsharing.h API/message_queues.h API/semsharing.h 
OBJECTS2 = ArrayList/ArrayList.o Protocol/net_protocol.o API/coordinator.o API/fmmap.o API/memsharing.o API/message_queues.o API/semsharing.o Client/client.o 

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.c $(HEADERS2)
	$(CC) -c -o $@ $< $(CFLAGS)

all: fmmap_server client

fmmap_server: $(OBJECTS)
	gcc -o $@ $^ $(CFLAGS)

client: $(OBJECTS2)
	gcc -o $@ $^ $(CFLAGS) 

clean:
	rm -f $(OBJECTS)
	rm -f $(OBJECTS2)
	rm -f fmmap_server
	rm -f client
	

