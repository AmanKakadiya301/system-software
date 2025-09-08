/*
============================================================================
Name : 03.c
Author : Aman Kakadiya
Description : Write a program to create a file and print the file descriptor value. Use creat() system call
Date: 22nd Aug, 2025.
============================================================================
*/

#include<sys/types.h>	// Import data type used in System call
#include<sys/stat.h>	// Import sys/types.h data micros
#include<fcntl.h>		// Import file Control system calls
#include<stdio.h>		// Import printf and perror

int main(int argc, char *argv[]){
	if(argc != 2){
		perror("Enter File Name in arguement\n");
		return 0;
	}
	
	const char *fileName = argv[1];
	int fd = creat(fileName,S_IRWXU);

	if(fd == -1){
		perror("Error while creating a file\n");
	}else{
		printf("The file have been successfully create and the file descriptor value is: %d\n",fd);
	}
	return 0;
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out newFile
The file have been successfully create and the file descriptor value is: 3
*/
