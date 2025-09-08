/*
============================================================================
Name : 12.c
Author : Aman Kakadiya
Description : Write a program to find out the opening mode of a file. Use `fcntl`
Date: 25th Aug, 2025.
============================================================================
*/


#include <sys/types.h> // Import for `open` system call
#include <sys/stat.h>  // Import for `open` system call
#include <fcntl.h>     // Import for `open`, `fcntl` system call
#include <unistd.h>    // Import for `fcntl` system call
#include <stdio.h>     // Import required for `perror` & `printfi` function

int main(int argc, char *argv[]){
	if(argc < 2){
		perror("Enter File Name\n");
		return 0;
	}
	int fd = open(argv[1],O_RDWR);
	if(fd == -1){
		perror("Error while opening file\n");
		return 0;
	}
	
	int status = fcntl(fd,F_GETFL);
	if(status == -1){
		printf("ERROR in Status\n");
		return 0;
	}
	 /*
                    0    = O_RDONLY
                    1    = O_WRONLY
                    2    = O_RDWR
                    64   = O_CREAT
                    512  = O_TRUNC
                    1024 = O_APPEND
                    577  = O_WRONLY | O_CREAT | O_TRUNC
                    1089 = O_WRONLY | O_CREAT | O_APPEND
                    578  = O_RDWR   | O_CREAT | O_TRUNC
                    1090 = O_RDWR   | O_CREAT | O_APPEND
         */
	switch (O_ACCMODE & status){
        case 0:
            printf("The file has been opened with the flags : O_RDONLY\n");
            break;
        case 1:
            printf("The file has been opened with the flags : O_WRONLY\n");
            break;
        case 2:
            printf("The file has been opened with the flags : O_RDWR\n");
            break;
            
		// If other were to be included
        case 64:
            printf("The file has been opened with the flags : O_CREAT\n");
            break;
        case 512:
            printf("The file has been opened with the flags : O_TRUNC\n");
            break;
        case 1024:
            printf("The file has been opened with the flags : O_APPEND\n");
            break;
        case 577:
            printf("The file has been opened with the flags : O_WRONLY | O_CREAT | O_TRUNC\n");
            break;
        case 1089:
            printf("The file has been opened with the flags : O_WRONLY | O_CREAT | O_APPEND\n");
            break;
        case 578:
            printf("The file has been opened with the flags : O_RDWR   | O_CREAT | O_TRUNC\n");
        	break;
        case 1090:
            printf("The file has been opened with the flags : O_RDWR   | O_CREAT | O_APPEND\n");
            break;
        default:
            printf("Error!");
        }
	return 0;	
}
/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out test.txt
The file has been opened with the flags : O_RDWR
*/