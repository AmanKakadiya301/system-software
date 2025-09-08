/*
============================================================================
Name : 01_c.c
Author : Aman Kakadiya
Description : Create a FIFO file using the `mkfifo` library function or `mknod` system call
Date: 22nd Aug, 2025.
============================================================================
*/

#include<sys/types.h> // Import data type used in System call
#include<sys/stat.h>  // Import micros for data type in System call
#include<stdio.h>  

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("Enter 2 file names to create FIFO with mkfifo and mknod\n");
		return 0;
	}	
	const char *mkfifoName = argv[1];
	const char *mknodName = argv[2];

	int mkfifo_status = mkfifo(mkfifoName, S_IRWXU);
	
	if(mkfifo_status == -1){
		perror("Error while creating FIFO file with mkfifo\n");
	}else{
		printf("Created FIFO file with mkfifo successfully\n");
	}

	int mknod_status = mknod(mknodName, S_IFIFO | S_IRWXU ,0); // 3rd arguement is for dev_t which is not required for FIFO file

	if(mknod_status == -1){
		perror("Error while creating FIFO file with mknod\n");
	}else{
		printf("Created FIFO file with mknod successfully\n");
	}
}

/* 
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out mkfifo mknod
Created FIFO file with mkfifo successfully
Created FIFO file with mknod successfully
*/