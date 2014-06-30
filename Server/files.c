#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "files.h"

int openFile(char* pathname)
{
   	if(Files == NULL)
   	{
		Files = malloc(sizeof(ArrayList));
		initArrayList(Files);
    	}
    	file * file = malloc(sizeof(file));

	int fin;
	fin = open(pathname);
    	if(fin == -1)
    	{
		return -1;
    	}
    	else
    	{
    		file->fd = fin;
		file->pathname = pathname;
    		add(Files, file);

		return fin;
	}
}

int closeFile(int fd)
{
	return close(fd);
}

int readFile(char* buff, size_t length, int fd, off_t offset)
{
	//fseek(f, offset, SEEK_SET);
	ssize_t bytes;
	bytes = read(fd, buff, length);

	if(bytes == -1)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}
