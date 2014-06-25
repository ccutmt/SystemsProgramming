#include "fmmap.h"
#include "coordinator.h"
#include "ArrayList.h"

struct map_info{
	int mapid;
	ArrayList *offsets;
	int offset;
	struct in_addr ip;
	int port;
};
typedef struct map_info map_info;

ArrayList *addressmap = NULL;
int init = -1;
int idcount = -1;

void * rmmap(fileloc_t location, off_t offset) {
	if(init == -1){
		if(initCoordinator() == -1){
			return (void*) -1;
		}
	}

	rm_protocol *tosend = malloc(sizeof(rm_protocol));
	rm_protocol *reply = malloc(sizeof(rm_protocol));
	makeMapRequest(tosend, getpid(), location.pathname, offset);

	if(is_master == 0){
		int sfd = getServerFd(location.ipaddress, location.port);
		requestServer(tosend, reply, sfd);
	}
	else{
		rqst_over_queue *tosend_queue = malloc(sizeof(rqst_over_queue));
		rqst_over_queue *reply_queue = malloc(sizeof(rqst_over_queue));

		requestRead(sem_header_set);
		int read = getSharedInt(_header_memory);
		tosend_queue->pid = (long)read;
		releaseRead(sem_header_set);

		tosend_queue->message = *tosend;
		tosend_queue->ipaddress = location.ipaddress;
		tosend_queue->port = location.port;
		sendMsg(tosend_queue);
		free(tosend_queue);

		/*
		 * block until reply
		 */
		waitForSignal();

		if(receiveMsgQueue(_queue_id, reply_queue) == 0)
			memcpy(reply, &reply_queue->message, sizeof(rm_protocol));
		else perror("Message not received");
	}

	if(reply->type == ERROR){
		free(reply);
		return (void*) -1;
	}
	else{
		requestWrite(sem_data_set);

		int check = getFilePartOffset(reply->filepart, location.ipaddress.s_addr);
		if(check == -1){
			check = makeMap(reply, location.ipaddress);
		}
		else incrementUsers(check);

		releaseWrite(sem_data_set);

		idcount++;
		map_info *new = malloc(sizeof(map_info));
		new->mapid = idcount;
		new->offset = offset;
		new->offsets = malloc(sizeof(ArrayList));
		new->port = location.port;
		memcpy(&new->ip, &location.ipaddress, sizeof(struct in_addr));
		initArrayList(new->offsets);
		int *off = malloc(sizeof(int));
		*off = check;
		add(new->offsets, off);

		if(addressmap == NULL){
			addressmap = malloc(sizeof(ArrayList));
			initArrayList(addressmap);
		}
		add(addressmap, new);

		free(reply);
		return (void*) &new->mapid;
	}

}



int rmunmap(void *addr){
	int c = 0;
	int offset = -1;
	while(c <= addressmap->current){
		if(addr == getElement(addressmap, c)){
			offset = c;
			break;
		}
		c++;
	}

	if(offset == -1)
		return -1;
	else{
		int i = 0;
		int removed = 0;
		map_info* target = getElement(addressmap, offset);
		requestWrite(sem_data_set);
		for(i = 0; i <= addressmap->current; i++){
			removed += makeUnmap(*(int*)getElement(target->offsets, i), target->ip, target->port);
		}
		releaseWrite(sem_data_set);
	}

	return 0;
}
