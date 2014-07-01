#include "files.h"
#include "ArrayList.h"

int openFile(char* pathname, int *err)
{
	int fin = open(pathname, O_RDWR);
	if(fin == -1){
		*err = errno;
	}
	return fin;
}

int closeFile(int fd, int *err)
{
	int res = close(fd);
	if(res == -1)
		*err = errno;
	return res;
}

ssize_t readFile(char* buff, size_t length, int fd, off_t offset, int *err)
{
	off_t check;
	ssize_t bytes;

	check = lseek(fd, offset, SEEK_SET);

	if(check == -1)
	{
		*err = errno;
		return -1;
	}

	bytes = read(fd, buff, length);

	if(bytes == -1)
	{
		*err = errno;
	}
	return bytes;
}

ssize_t writeFile(char *buff, size_t length, int fd, off_t offset, int *err){
	off_t check;
	ssize_t bytes;

	check = lseek(fd, offset, SEEK_SET);

	if(check == -1)
	{
		*err = errno;
		return -1;
	}

	bytes = write(fd, buff, length);

	if(bytes == -1)
	{
		*err = errno;
	}
	return bytes;
}
