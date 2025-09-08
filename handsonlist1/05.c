/*
============================================================================
Name : 05.c
Author : Aman Kakadiya
Description : Write a program to create five new files with infinite loop. Execute the program in the background and check the file descriptor table at /proc/pid/fd.
Date: 23rd Aug, 2025.
============================================================================
*/

#include <sys/types.h>	// Import for `creat` system call
#include <sys/stat.h>	// Import micro values
#include <fcntl.h>	// Import file control
#include <stdio.h>	// Import printf
#include <unistd.h>

int main(){
	while(1){
		int fd1 = creat("test1.txt",S_IRWXU);
		int fd2 = creat("test2.txt",S_IRWXU);
		int fd3 = creat("test3.txt",S_IRWXU);
		int fd4 = creat("test4.txt",S_IRWXU);
		int fd5 = creat("test5.txt",S_IRWXU);

		printf("%d %d %d %d %d\n",fd1,fd2,fd3,fd4,fd5);
		close(fd1);
		close(fd2);
		close(fd3);
		close(fd4);
		close(fd5);
		sleep(5);
	}
}

/* Output
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
3 4 5 6 7
3 4 5 6 7
3 4 5 6 7
3 4 5 6 7
*/
