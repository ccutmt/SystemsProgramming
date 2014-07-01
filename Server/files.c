#include "files.h"
#include "ArrayList.h"

int openFile(char* pathname)
{
	int fin = open(pathname, O_RDWR);
	return fin;
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
		perror("Seek error");
		return -1;
	}

	bytes = read(fd, buff, length);

	if(bytes == -1)
	{
		perror("Read error");
		return -1;
	}
	else
	{
		perror("it is 0");
		return 0;
	}
}

int writeFile(char *buff, size_t length, int fd, off_t offset){
	off_t check;
	ssize_t bytes;

	check = lseek(fd, offset, SEEK_SET);

	if(check == -1)
	{
		return -1;
	}

	bytes = write(fd, buff, length);

	if(bytes == -1)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}
