/*
============================================================================
Name : 11_b.c
Author : Aman Kakadiya
Description : Write a program to open a file, duplicate the file descriptor and append the file with both the descriptors and check whether the file is updated properly or not.
			Use dup2
Date: 25th Aug, 2025.
============================================================================
*/

#include <unistd.h>    // Import for `dup` system call
#include <sys/types.h> // Import for `open` system call
#include <sys/stat.h>  // Import for `open` system call and micros
#include <fcntl.h>     // Import File Control
#include <stdio.h>     // Import required for `perror` & `perror`

int main(int argc, char *argv[]){
	if(argc<2){
		perror("File Name not given\n");
		return 0;
	}

	int fd = open(argv[1],O_WRONLY | O_APPEND);

	if(fd == -1){
		perror("Error while opening file\n");
		return 0;
	}

	int fd_dup = dup2(fd,111);

	if(fd_dup == -1){
		perror("Error while duplication FD\n");
		return 0;
	}
	
	int writeByte = write(fd,"ORIGINAL",8);
	if(writeByte == -1){
		perror("Error while writing into ORIGINAL\n");
		return 0;
	}

	writeByte = write(fd_dup,"DUPLICATE",9);
	if(writeByte == -1){
		perror("Error while writing into DUP\n");
		return 0;
	}
	return 0;
}
/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ cat test.txt
Hello World
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out test.txt
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ cat test.txt
Hello WorldORIGINALDUPLICATE
*/
