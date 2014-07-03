#include "fmmap.h"
#include "coordinator.h"
#include "../ArrayList/ArrayList.h"

int init = -1;

void setSignalMaskUsr1Only(){
	sigset_t old;
	sigprocmask(SIG_SETMASK, &sigusr1only, &old);
}

void unblockAllSignals(){
	sigset_t old;
	sigprocmask(SIG_SETMASK, &all, &old);
}

void * rmmap(fileloc_t location, off_t offset) {
	if(init == -1){
		if(initCoordinator() == -1){
			return (void*) -1;
		}else init = 1;
	}

	rm_protocol *tosend = malloc(sizeof(rm_protocol));
	rm_protocol *reply = malloc(sizeof(rm_protocol));
	makeMapRequest(tosend, getpid(), location.pathname, offset);

	setSignalMaskUsr1Only();
	makeRequest(tosend, reply, location.ipaddress, location.port);
	unblockAllSignals();

	if(reply->type == ERROR){
		errno = reply->error_id;
		free(reply);
		return (void*) -1;
	}
	else{
		setSignalMaskUsr1Only();
		requestWrite(sem_data_set);

		unsigned long real_offset = offset / _DATA_LENGTH;
		real_offset = real_offset * _DATA_LENGTH;

		int check = getFilePartOffset(reply->filepart, location.ipaddress.s_addr, real_offset);
		if(check == -1){
			check = makeMap(reply, location.ipaddress, real_offset);
		}
		else incrementUsers(check);

		releaseWrite(sem_data_set);
		unblockAllSignals();

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
	unsigned long read_c = 0;
	if(init == -1){
		return read_c;
	}

	int off = getMappingByAddr(addr);

	if(off == -1){
		return read_c;
	}
	else{
		int requests = (count + _DATA_LENGTH - 1) / _DATA_LENGTH;
		int i;
		map_info *mapping = getElement(addressmap, off);
		unsigned long mapped_offset = mapping->offset;
		unsigned long new_offset = mapped_offset + offset;
		unsigned long temp_offset = new_offset;
		unsigned long remaining = count;
		unsigned long curr_off;

		setSignalMaskUsr1Only();
		requestWrite(sem_data_set);

		for(i = 0; i < requests; i++){
			unsigned long start_offset = ((new_offset + (_DATA_LENGTH * i))/_DATA_LENGTH) * _DATA_LENGTH ;
			int newmap = makeRead(mapping->fileid, start_offset, mapping->ip, mapping->port);
			if(newmap == -1){
				releaseWrite(sem_data_set);
				unblockAllSignals();
				return read_c;
			}
			else{
				//Search for entry
				int off = checkIfRead(mapping, newmap);

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
			read_c += tocopy;
		}

		releaseWrite(sem_data_set);
		unblockAllSignals();
	}
	return read_c;
}

ssize_t mwrite(void *addr, off_t offset, void *buff, size_t count){
	unsigned long count_c = 0;
	if(init == -1){
		return count_c;
	}

	int off = getMappingByAddr(addr);

	if(off == -1)
		return count_c;
	else{
		int requests = (count + _DATA_LENGTH - 1) / _DATA_LENGTH;
		int i;
		map_info *mapping = getElement(addressmap, off);
		unsigned long mapped_offset = mapping->offset;
		unsigned long new_offset = mapped_offset + offset;
		unsigned long temp_offset = new_offset;
		unsigned long remaining = count;
		unsigned long curr_off;

		setSignalMaskUsr1Only();
		requestWrite(sem_data_set);

		for(i = 0; i < requests; i++){
			unsigned long start_offset = ((new_offset + (_DATA_LENGTH * i))/_DATA_LENGTH) * _DATA_LENGTH ;
			int writeto = getFilePartOffset(mapping->fileid, mapping->ip.s_addr,start_offset);

			if(writeto == -1){
				releaseWrite(sem_data_set);
				return count_c;
			}
			else{
				int off = checkIfRead(mapping, writeto);

				//If no entries found, create one. Otherwise, update timestamp
				if(off == -1){
					releaseWrite(sem_data_set);
					return count_c;
				}else{
					map_part_info *toupdate = getElement(mapping->offsets, off);
					_shared_file read;
					readSharedData(writeto, &read);

					if(read.write_timestamp > toupdate->timestamp){
						releaseWrite(sem_data_set);
						return count_c;
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
						makeWriteRequest(&tosend, mapping->fileid, getpid(), read.data, start_offset, tocopy);
						makeRequest(&tosend, &reply, mapping->ip, mapping->port);

						if(reply.type == ERROR && reply.error_id == -1){
							getUpdatedDataFromServer(mapping->fileid, start_offset, &read, mapping->ip, mapping->port, writeto);
						}else if(reply.type == ERROR){
							errno = reply.error_id;
						}else{
							count_c += tocopy;
						}
					}
				}
			}
		}

		releaseWrite(sem_data_set);
		unblockAllSignals();
	}
	return count_c;
}

int rmunmap(void *addr){
	if(init == -1){
		return -1;
	}

	int offset = getMappingByAddr(addr);

	if(offset == -1)
		return -1;
	else{
		int i = 0;
		map_info* target = getElement(addressmap, offset);
		setSignalMaskUsr1Only();
		requestWrite(sem_data_set);
		for(i = 0; i <= target->offsets->current; i++){
			map_part_info *part = (map_part_info*)(getElement(target->offsets, i));
			if(makeUnmap(part->part_offset, target->ip, target->port) == -1)
				return -1;
		}
		releaseWrite(sem_data_set);
		unblockAllSignals();
	}

	return 0;
}
