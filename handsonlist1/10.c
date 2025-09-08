/*
============================================================================
Name : 10.c
Author : Aman Kakadiya
Description : Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10 bytes (use lseek) and write again 10 bytes. 
    1. Check the return value of `lseek` 
    2. Open the file with `od` and check the empty spaces in between the data.
Date: 24th Aug, 2025.
============================================================================
*/

#include <sys/types.h>	// Import open
#include <sys/stat.h>	// Import micros
#include <fcntl.h>	// Import file control
#include <unistd.h>	// Import write
#include <stdio.h>	// Import perror and printf

int main(int argc, char *argv[]){
	if(argc < 2){
		perror("File Name not given\n");
		return 0;
	}

	int fd = open(argv[1],O_RDWR);
	if(fd == -1){
		perror("Error while opening file\n");
	}

	char *buffer = "0123456789";
	int writeByte = write(fd,buffer,10);
	if(writeByte == -1){
		perror("Error while writing\n");
		return 0;
	}
	lseek(fd,10,SEEK_CUR); // Moves 10 bytes;
	writeByte = write(fd,buffer, 10);
	if(writeByte == -1){
		perror("Error while writing\n");
		return 0;
	}

	int offset = lseek(fd,0,SEEK_CUR);
	printf("Current offset: %d\n",offset);
	return 0;
	return 0;
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out test.txt
Current offset: 30
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ cat test.txt
0123456789D, HOW ARE0123456789
*/
