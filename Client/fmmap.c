#include "fmmap.h"
#include "coordinator.h"
#include "ArrayList.h"

struct map_info{
	char *path;
	int offset;
};

ArrayList *addressmap = NULL;


void * rmmap(fileloc_t location, off_t offset) {
	int sfd = getServerFd(location.ipaddress, location.port);
	rm_protocol *tosend = malloc(sizeof(rm_protocol));
	rm_protocol *reply = malloc(sizeof(rm_protocol));
	makeMapRequest(tosend, getpid(), location->pathname, offset);

	if(is_master == 0){
		requestServer(tosend, reply, sfd);
	}
	else{
		rqst_over_queue *tosend_queue = malloc(sizeof(rqst_over_queue));
		rqst_over_queue *reply_queue = malloc(sizeof(rqst_over_queue));
		tosend_queue->pid = getpid();
		tosend_queue->message = *tosend;
		sendMsg(tosend_queue);
		free(tosend_queue);
		/*
		 * block until reply
		 */
		if(receiveMsgQueue(_queue_id, reply_queue) == 0)
			printf("received successfully");
	}

	if(reply->type == ERROR)
		return (void*) -1;
	else{

	}
	return (void*) -1;
}

int rmunmap(void *addr){

	return 0;
}
