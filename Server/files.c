#include "files.h"
#include "ArrayList.h"

int openFile(char* pathname)
{
   	if(Files == NULL)
   	{
		Files = malloc(sizeof(ArrayList));
		initArrayList(Files);
    	}
    	file * file = malloc(sizeof(file));

	int fin;
	fin = open(pathname, O_RDWR);
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
	off_t check;
	ssize_t bytes;

	check = lseek(fd, offset, SEEK_SET);

	if(check == -1)
	{
		return -1;
	}

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
