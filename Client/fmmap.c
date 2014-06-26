#include "fmmap.h"
#include "coordinator.h"
#include "ArrayList.h"

struct map_info{
	int mapid;				//Unique id given to user
	unsigned int fileid;	//File id given by server
	ArrayList *offsets;		//List of map_part_info
	int offset;				//Offset requested by user
	struct in_addr ip;
	int port;
};
typedef struct map_info map_info;

typedef struct map_part_info{
	unsigned long part_offset;		//Offset in shared memory
	unsigned int timestamp;			//Last read time

} map_part_info;

ArrayList *addressmap = NULL;
int init = -1;

//used to generate ids for mapping. These are returned to the user
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

		unsigned long real_offset = offset / _DATA_LENGTH;
		real_offset = real_offset * _DATA_LENGTH;

		int check = getFilePartOffset(reply->filepart, location.ipaddress.s_addr, real_offset);
		if(check == -1){
			check = makeMap(reply, location.ipaddress, real_offset);
		}
		else incrementUsers(check);

		releaseWrite(sem_data_set);

		idcount++;

		//Create file part info
		map_part_info *newpart = malloc(sizeof(map_part_info));
		newpart->part_offset = check;
		newpart->timestamp = time(NULL);

		//create mapping entry
		map_info *new = malloc(sizeof(map_info));
		new->mapid = idcount;
		new->offset = offset;
		new->offsets = malloc(sizeof(ArrayList));
		new->port = location.port;
		new->fileid = reply->filepart;
		memcpy(&new->ip, &location.ipaddress, sizeof(struct in_addr));
		initArrayList(new->offsets);
		add(new->offsets, newpart);

		//Add mapping entry to list of mappings
		if(addressmap == NULL){
			addressmap = malloc(sizeof(ArrayList));
			initArrayList(addressmap);
		}
		add(addressmap, new);

		free(reply);
		return (void*) &new->mapid;
	}

}

ssize_t mread(void *addr, off_t offset, void *buff, size_t count){
	int c = 0;
	int off = -1;
	while(c <= addressmap->current){
		if(addr == getElement(addressmap, c)){
			off = c;
			break;
		}
		c++;
	}

	if(off == -1)
		return -1;
	else{
		int requests = (count + _DATA_LENGTH - 1) / _DATA_LENGTH;
		int i;
		map_info *mapping = getElement(addressmap, off);
		unsigned long mapped_offset = mapping->offset;
		unsigned long new_offset = mapped_offset + offset;
		unsigned long temp_offset = new_offset;
		unsigned long remaining = count;
		unsigned long curr_off;

		requestWrite(sem_data_set);

		for(i = 0; i < requests; i++){
			unsigned long start_offset = ((new_offset + (_DATA_LENGTH * i))/_DATA_LENGTH) * _DATA_LENGTH ;
			int newmap = makeRead(mapping->fileid, start_offset, mapping->ip, mapping->port);
			if(newmap == -1){
				releaseWrite(sem_data_set);
				return -1;
			}
			else{
				//Search for entry
				int c = 0;
				int off = -1;
				while(c <= mapping->offsets->current){
					map_part_info *query = getElement(mapping->offsets, c);
					if(newmap == query->part_offset){
						off = c;
						break;
					}
					c++;
				}

				//If no entries found, create one. Otherwise, update timestamp
				if(off == -1){
					map_part_info *newpart = malloc(sizeof(map_part_info));
					newpart->part_offset = newmap;
					newpart->timestamp = time(NULL);
					add(mapping->offsets, newpart);
				}else{
					map_part_info *toupdate = getElement(mapping->offsets, off);
					toupdate->timestamp = time(NULL);
				}
			}

			_shared_file copy;
			readSharedData(newmap, &copy);

			unsigned long tocopy = 0;

			//copy data into array
			long long diff = remaining - _DATA_LENGTH;
			if(diff < 0){
				tocopy = remaining;
			}
			else tocopy = _DATA_LENGTH;

			curr_off = temp_offset - start_offset;

			memcpy(buff + (count - remaining), &copy.data[curr_off], tocopy);
			remaining -= tocopy;
			temp_offset += tocopy - curr_off;
		}

		releaseWrite(sem_data_set);
	}
	return count;
}

ssize_t mwrite(void *addr, off_t offset, void *buff, size_t count){
	int c = 0;
	int off = -1;
	while(c <= addressmap->current){
		if(addr == getElement(addressmap, c)){
			off = c;
			break;
		}
		c++;
	}

	if(off == -1)
		return -1;
	else{
		int requests = (count + _DATA_LENGTH - 1) / _DATA_LENGTH;
		int i;
		map_info *mapping = getElement(addressmap, off);
		unsigned long mapped_offset = mapping->offset;
		unsigned long new_offset = mapped_offset + offset;
		unsigned long temp_offset = new_offset;
		unsigned long remaining = count;
		unsigned long curr_off;

		requestWrite(sem_data_set);

		for(i = 0; i < requests; i++){
			unsigned long start_offset = ((new_offset + (_DATA_LENGTH * i))/_DATA_LENGTH) * _DATA_LENGTH ;
			int writeto = getFilePartOffset(mapping->fileid, mapping->ip.s_addr,start_offset);

			if(writeto == -1){
				releaseWrite(sem_data_set);
				return -1;
			}
			else{
				//Check if file part is read by user
				int c = 0;
				int off = -1;
				while(c <= mapping->offsets->current){
					map_part_info *query = getElement(mapping->offsets, c);
					if(writeto == query->part_offset){
						off = c;
						break;
					}
					c++;
				}

				//If no entries found, create one. Otherwise, update timestamp
				if(off == -1){
					releaseWrite(sem_data_set);
					return -1;
				}else{
					map_part_info *toupdate = getElement(mapping->offsets, off);
					_shared_file read;
					readSharedData(writeto, &read);

					if(read.write_timestamp > toupdate->timestamp){
						releaseWrite(sem_data_set);
						return -1;
					}else{
						toupdate->timestamp = time(NULL);
						read.write_timestamp = toupdate->timestamp;

						unsigned long tocopy = 0;

						//copy data into array
						long long diff = remaining - _DATA_LENGTH;
						if(diff < 0){
							tocopy = remaining;
						}
						else tocopy = _DATA_LENGTH;

						curr_off = temp_offset - start_offset;

						memcpy(&read.data[curr_off], buff + (count - remaining), tocopy);
						remaining -= tocopy;
						temp_offset += tocopy - curr_off;

						rm_protocol tosend, reply;
						makeWriteRequest(&tosend, mapping->fileid, getpid(), read.data, start_offset, _DATA_LENGTH);
						if(is_master == 0){
							requestServer(&tosend, &reply, getServerFd(mapping->ip, mapping->port));
						}else{
							rqst_over_queue *tosend_queue = malloc(sizeof(rqst_over_queue));
							rqst_over_queue *reply_queue = malloc(sizeof(rqst_over_queue));

							requestRead(sem_header_set);
							int read = getSharedInt(_header_memory);
							tosend_queue->pid = (long)read;
							releaseRead(sem_header_set);

							tosend_queue->message = tosend;
							tosend_queue->ipaddress = mapping->ip;
							tosend_queue->port = mapping->port;
							sendMsg(tosend_queue);
							free(tosend_queue);

							waitForSignal();

							if(receiveMsgQueue(_queue_id, reply_queue) == 0)
								memcpy(&reply, &reply_queue->message, sizeof(rm_protocol));
							else perror("Message not received");
						}
					}

				}
			}
		}

		releaseWrite(sem_data_set);
	}
	return count;
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
			map_part_info *part = (map_part_info*)(getElement(target->offsets, i));
			removed += makeUnmap(part->part_offset, target->ip, target->port);
		}
		releaseWrite(sem_data_set);
	}

	return 0;
}
