/*
============================================================================
Name : 04.c
Author : Aman Kakadiya
Description : Write a program to open an existing file with read write mode. Try O_EXCL flag also.
Date: 23rd Aug, 2025.
============================================================================
*/

#include <sys/types.h>	// Import data types used in System Call
#include <sys/stat.h>	// Import System Call micro values
#include <fcntl.h>	// Import File Control System calls
#include <stdio.h>	// Import printf and perror

int main(int argc, char *argv[]){
	if(argc != 2){
		perror("File Name not found\n");
		return 0;
	}

	const char *fileName = argv[1];
	int fd = open(fileName, O_RDWR | O_EXCL);	// If we use O_CREAT | O_EXCL, it works exclusively, meaning fd will return -1 due to file being present, else if file does not exist, it creates new file and return fd. If used O_CREAT without O_EXCL, it will return fd whether file is present or not

	if(fd == -1){
		perror("Error while opening the file!\n");
	}else{
		printf("Sucessfully opened file with Read & Write Mode\n");
	}
	return 0;
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out 04.c
Sucessfully opened file with Read and Write Mode

*/