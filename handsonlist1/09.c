/*
============================================================================
Name : 09.c
Author : Aman Kakadiya
Description : Write a program to print the following information about a given file - 
    1. Inode 
    2. Number of hard links 
    3. UID 
    4. GID 
    5. Size 
    6. Block size 
    7. Number of blocks 
    8. Time of last access 
    9. Time of last modification 
    10. Time of last change
Date: 24th Aug, 2025.
============================================================================
*/

#include <sys/types.h> // Import for open system call.
#include <sys/stat.h>  // Import micros
#include <fcntl.h>     // Import File Control
#include <unistd.h>    // Import for read and write system call
#include <stdio.h>     // Import for perrro and printf functions
#include <time.h>

int main(int argc, char *argv[]){
	if(argc < 2){
		fprintf(stderr,"Usage: %s <filename>\n",argv[0]);
		return 1;
	}

	struct stat statbuf;

	if((stat(argv[1],&statbuf)) == -1){
		perror("Error while executing the file!\n");
		return 1;
	}

	printf("Inode: %lu\n",(unsigned long)statbuf.st_ino);
	printf("Number of hard links: %lu\n", (unsigned long)statbuf.st_nlink);
	printf("UID: %u\n", statbuf.st_uid);
	printf("GID: %u\n", statbuf.st_gid);
	printf("Size: %ld bytes\n", (long)statbuf.st_size);
	printf("Block Size: %ld\n", (long)statbuf.st_blksize);
	printf("Number of Blocks: %ld\n", (long)statbuf.st_blocks);
	// Use the `ctime` function in `time.h` header to convert the timestamp in epoch to a more human readable form
	printf("Last Access: %s", ctime(&statbuf.st_atime));
	printf("Last Modification: %s", ctime(&statbuf.st_mtime));
	printf("Time of Last change: %s", ctime(&statbuf.st_ctime));
	return 0;
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out 09.c 
Inode: 3280181
Number of hard links: 1
UID: 1000
GID: 1000
Size: 1976 bytes
Block Size: 4096
Number of Blocks: 8
Last Access: Sun Sep  7 14:15:56 2025
Last Modification: Sun Sep  7 14:15:56 2025
Time of Last change: Sun Sep  7 14:15:56 2025
*/