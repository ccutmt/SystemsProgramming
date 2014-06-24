#include "fmmap.h"
#include "coordinator.h"
#include "ArrayList.h"

struct map_info{
	int mapid;
	ArrayList *offsets;
	int offset;
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
		sigset_t mask;

		sigfillset(&mask);
		sigdelset(&mask, SIGUSR1);
		//sigprocmask(SIGUSR1, &mask, &oldmask);
		sigsuspend(&mask);

		if(receiveMsgQueue(_queue_id, reply_queue) == 0)
			memcpy(&reply_queue->message, reply, sizeof(rm_protocol));
		else perror("Message not received");
		//sigprocmask(SIG_SETMASK, &oldmask)
	}

	if(reply->type == ERROR)
		return (void*) -1;
	else{
		printf("no error");
		requestWrite(sem_data_set);

		printf("no not here");
		int check = getFilePartOffset(reply->filepart, location.ipaddress.s_addr);
		if(check == -1){
			printf("not found");
			check = makeMap(reply, location.ipaddress);
		}

		releaseWrite(sem_data_set);

		printf("pid: %i\nFilepart: %ui\nType: %i\nErrorId: %i\nPath: %s\ncount: %i\nData: %s\n", reply->pid, reply->filepart,
				reply->type , reply->error_id, reply->path, reply->count, reply->data);

		idcount++;
		map_info *new = malloc(sizeof(map_info));
		new->mapid = idcount;
		new->offset = offset;
		new->offsets = malloc(sizeof(ArrayList));
		initArrayList(new->offsets);
		int *off = malloc(sizeof(int));
		*off = check;
		add(new->offsets, off);

		if(addressmap == NULL){
			addressmap = malloc(sizeof(ArrayList));
			initArrayList(addressmap);
		}
		add(addressmap, new);
		return (void*)&new->mapid;
	}

}

int rmunmap(void *addr){

	return 0;
}
