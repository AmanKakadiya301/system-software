/*
============================================================================
Name : 22.c
Author : Aman Kakadiya
Description : Write a program, open a file, call fork, and then write to the file by both the child as well as the parent processes. Check output of the file
Date: 26th Aug, 2025.
============================================================================
*/

#include <unistd.h>    	// Import for `fork` system call
#include <sys/types.h> 	// Import for `open`, `fork` system call
#include <sys/stat.h>  	// Import for `open` system call
#include <fcntl.h>     	// Import for `open` system call
#include <stdio.h> 	// Import `perror`
int main(int argc, char *argv[]){
	if(argc < 2){
		perror("Enter File Name\n");
		return 0;
	}
	int fd;
	if((fd = open(argv[1],O_RDWR | O_CREAT | O_APPEND)) == -1){
		perror("FD\n");
		return 0;
	}
	if(fork()){
		write(fd,"PARENT\n",7);
	}else{
		write(fd,"CHILD\n",6);
	}
	close(fd);
	return 0;
}


/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ cc 22.c
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out b.txt
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ cat b.txt
PARENT
CHILD
*/