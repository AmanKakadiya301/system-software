/*
============================================================================
Name : 07.c
Author : Aman Kakadiya
Description : Write a program to copy file1 into file2 ($cp file1 file2).
Date: 24th Aug, 2025.
============================================================================
*/

#include <sys/types.h>	// Import system call data types
#include <sys/stat.h>	// Import micro for system call
#include <fcntl.h>	// Import file control
#include <unistd.h>	// Import read and write
#include <stdio.h>	// Import perror

int main(int argc, char *argv[]){
	if(argc < 3){
		perror("Source and Destination File to be entered\n");
		return 0;
	}
	char *buffer[800];
	int readByte;
	int writeByte;

	int sourceFD = open(argv[1],O_RDONLY);
	int destinationFD = -1;
	if(sourceFD != -1) destinationFD = open(argv[2], O_CREAT | O_RDWR, S_IRWXU);

	if(destinationFD == -1){
		perror("Error in source / destination File \n");
		return 0;
	}

	while((readByte = read(sourceFD,buffer,1024)) > 0){
		writeByte = write(destinationFD, buffer, readByte);
		if(writeByte == -1){
			perror("Error in writing into file\n");
			return 0;
		}
	}
	if(readByte == -1){
		perror("Error in reading from source file\n");
	}else{
		printf("Successfully copied the file contents\n");
	}

	close(sourceFD);
	close(destinationFD);
	return 0;
}
/*
>amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
Source and Destination File to be entered
: Success
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out 01_a.c test.txt
Successfully copied the file contents

amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ cat test.txt 
/*

============================================================================
Name : 01_a.c
Author : Aman Kakadiya
Description : Create a soft link file using the `symlink` system call
Date: 22nd Aug, 2025.
============================================================================


#include<unistd.h> // import symlink
#include<stdio.h>  // import printf and perror

int main(int argc, char *argv[]){

	if(argc != 3){
		printf("Enter source and target file Name in arguement\n");
		return 0;
	}
	const char *source = argv[1];
	const char *target = argv[2];

	int status = symlink(source,target);
	if(status == -1){
		perror("symlink");
		printf("Error while creating link\n");
	}else{
		printf("Link created successfully\n");
	}
	return 0;
}

/*
>amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$./a.out source.txt softLink.txt 
>Link created successfully

*/
