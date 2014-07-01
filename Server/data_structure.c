#include "data_structure.h"

ArrayList * files;
unsigned long fileid;

void initDataStructure(){
	files = malloc(sizeof(ArrayList));
	initArrayList(files);
	fileid = 0;
}

filepart * getFilePart(off_t start_offset, rmfile * file){
	filepart * f = NULL;
	int c = 0;
	while(c <= file->fileparts->current){
		filepart *temp = getElement(file->fileparts, c);
		if(temp->start_offset == start_offset){
			return temp;
		}
		c++;
	}
	return f;
}

void setLastWriter(int user_fd, filepart *part){
	int c = 0;
	while(c <= part->entries->current){
		user_info *temp = getElement(part->entries, c);
		if(temp->conn->fd != user_fd){
			temp->isvalid = -1;
		}
		else temp->isvalid = 0;
		c++;
	}
}

rmfile * getFileById(unsigned long id){
	int c = 0;
	while(c <= files->current){
		rmfile *temp = getElement(files, c);
		if(temp->fileid == id){
			return temp;
		}
		c++;
	}
	return NULL;
}

rmfile * getFile(char *path){
	int c = 0;
	while(c <= files->current){
		rmfile *temp = getElement(files, c);
		if(strcmp(temp->pathname, path) == 0){
			return temp;
		}
		c++;
	}
	return NULL;
}

int getUserInfoOffset(connection *rem, filepart * fp){
	int c = 0;
	while(c <= fp->entries->current){
		user_info * temp = getElement(fp->entries, c);
		if(temp->conn == rem){
			return c;
		}
		c++;
	}
	return -1;
}

int getFilePartOffset(filepart * rem, rmfile *file){
	int c = 0;
	while(c <= file->fileparts->current){
		filepart * temp = getElement(file->fileparts, c);
		if(temp == rem){
			return c;
		}
		c++;
	}
	return -1;
}

int getRmfileOffset(rmfile *rem){
	int c = 0;
	while(c <= files->current){
		rmfile * temp = getElement(files, c);
		if(temp == rem){
			return c;
		}
		c++;
	}
	return -1;
}

void mapRequest(char * path, off_t offset, connection *conn, char *buff){
	rmfile * file = getFile(path);
	filepart *fp;
	user_info *ui;
	if(file == NULL){
		int fd = openFile(path);
		if(fd == -1)
			return;

		//Create file entry
		file = malloc(sizeof(filepart));
		file->fd = fd;
		file->pathname = malloc(sizeof(char) * strlen(path));
		memcpy(file->pathname, path, strlen(path));
		file->fileparts = malloc(sizeof(ArrayList));
		file->fileid = fileid +1;
		fileid++;
		initArrayList(file->fileparts);
	}

	fp = getFilePart(offset, file);
	if(fp == NULL){
		if(readFile(buff, _DATA_LENGTH, file->fd, offset) == 0){
			fp = malloc(sizeof(filepart));
			fp->start_offset = offset;
			fp->entries = malloc(sizeof(ArrayList));
			initArrayList(fp->entries);
			add(file->fileparts, fp);
		}
	}

	ui = malloc(sizeof(user_info));
	ui->conn = conn;
	ui->isvalid = 0;
	add(fp->entries, ui);
}

void unmapRequest(unsigned long id, off_t offset, connection *conn){
	rmfile * file = getFileById(id);
	filepart *fp;

	if(file == NULL)
		return;

	fp = getFilePart(offset, file);
	if(fp == NULL)
		return;

	int con_off = getUserInfoOffset(conn, fp);
	if(con_off == -1)
		return;
	else{
		user_info *rem = getElement(fp->entries, con_off);
		removeAt(fp->entries, con_off);
		free(rem);
	}

	if(fp->entries->current == -1){
		int fp_off = getFilePartOffset(fp, file);
		if(fp_off != -1){
			filepart *rem = getElement(file->fileparts, fp_off);
			removeAt(file->fileparts, fp_off);
			free(rem->entries);
			free(rem);
		}
	}

	if(file->fileparts->current == -1){
		int fileoff = getRmfileOffset(file);
		if(fileoff != -1){
			rmfile *rem = getElement(files, fileoff);
			removeAt(files, fileoff);
			free(rem->pathname);
			free(rem->fileparts);
			free(rem);
		}
	}
}

void readRequest(unsigned long id, off_t offset, size_t length, char * buff, connection *conn){
	rmfile * file = getFileById(id);
	filepart *fp;
	user_info *ui;

	if(file == NULL){
		return;
	}

	fp = getFilePart(offset, file);
	if(fp == NULL){
		if(readFile(buff, _DATA_LENGTH, file->fd, offset)){
			fp = malloc(sizeof(filepart));
			fp->start_offset = offset;
			fp->entries = malloc(sizeof(ArrayList));
			initArrayList(fp->entries);
			add(file->fileparts, fp);
		}
	}

	int con_off = getUserInfoOffset(conn, fp);
	if(con_off == -1){
		ui = malloc(sizeof(user_info));
		ui->conn = conn;
		ui->isvalid = 0;
		add(fp->entries, ui);
	}
	else{
		ui = getElement(fp->entries, con_off);
		ui->isvalid = 0;
	}
}

void writeRequest(unsigned long id, off_t offset, size_t length, char * buff, connection *conn){
	rmfile * file = getFileById(id);
	filepart *fp;

	if(file == NULL)
		return;

	fp = getFilePart(offset, file);
	if(fp == NULL)
		return;

	int con_off = getUserInfoOffset(conn, fp);
	if(con_off == -1){
		return;
	}
	else{
		user_info *ui = getElement(fp->entries, con_off);
		if(ui->isvalid == 0){
			writeFile(buff, length, file->fd, offset);
		}
		else return;
	}
}
