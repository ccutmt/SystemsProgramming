C = gcc
CFLAGS = -Wall

SERVER_OBJECTS = ArrayList/ArrayList.o Protocol/net_protocol.o Server/connections.o Server/data_structure.o Server/files.o Server/fmserver.o

COMMON_OBJECTS = ArrayList/ArrayList.o Protocol/net_protocol.o API/coordinator.o API/fmmap.o API/memsharing.o API/message_queues.o API/semsharing.o

CLIENT_OBJECTS = Client/client.o 


%.o: %.c %.h 
	$(CC) -c -o $@ $< $(CFLAGS)

all: fmmap_server client libfmmap.a

fmmap_server: $(SERVER_OBJECTS)
	gcc -o $@ $^ $(CFLAGS)

libfmmap.a: $(COMMON_OBJECTS)
	ar rcs $@ $(COMMON_OBJECTS)

client: $(COMMON_OBJECTS) $(CLIENT_OBJECTS)
	gcc -o $@ $^ $(CFLAGS) 

clean:
	rm -f $(SERVER_OBJECTS) $(COMMON_OBJECTS) $(CLIENT_OBJECTS)
	rm -f fmmap_server
	rm -f client
	rm -f libfmmap.a
