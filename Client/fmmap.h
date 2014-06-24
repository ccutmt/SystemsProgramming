#ifndef FMMAP
#define FMMAP

#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Type definition for a connection with a server
struct connection{
	struct in_addr ipaddress;
	int file_descriptor;
	struct file_entry *next;
};
typedef struct connection connection_t;

//Type definition representing a file on the server
struct file_entry{
	char *path;
	struct list *file_parts;
	struct file_entry *next;
};
typedef struct file_entry file_entry_t;

//Type definition for an entry for a file part
struct file_part{
	size_t offset;
	void *local_addr;
	struct file_part *next;
	struct rm_list *list;
};
typedef struct file_part file_part_t;

//Type definition for a list of users of a file
struct members{
	int p_id;
	struct members *next;
};
typedef struct members members_t;

//Type definition for a list
struct rm_list{
	void *head;
	int size;
};
typedef struct rm_list rm_list;

// Type definition for remote file location
struct fileloc {
    struct in_addr ipaddress; // Remote IP
    int port;                 // Remote port
    char *pathname;           // Remote file to be memory mapped
};
typedef struct fileloc fileloc_t;

// Create a new memory mapping in the virtual address space
// of the calling process         
// location: Remote file to be memory mapped
// offset:   File offset to memory map (from beginning of file)
//
// returns: Pointer to the mapped area. On error the value MAP_FAILED
//          ((void*) -1) is returned and errno is set appropriately
void *rmmap(fileloc_t location, off_t offset);

// Deletes mapping for the specified address range
// addr:   Address returned by mmap
// length: Length of mapping
//
// returns: 0 on success, -1 on failure and errno set appropriately
int rmunmap(void *addr);

// Attempt to read up to count bytes from memory mapped area pointed
// to by addr + offset into buff
//
// returns: Number of bytes read from memory mapped area
ssize_t mread(void *addr, off_t offset, void *buff, size_t count);

// Attempt to write up to count bytes to memory mapped area pointed
// to by addr + offset from buff
//
// returns: Number of bytes written to memory mapped area
ssize_t mwrite(void *addr, off_t offset, void *buff, size_t count);

#endif

