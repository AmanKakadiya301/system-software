/*
============================================================================
Name : 21.c
Author : Aman Kakadiya
Description : Write a program, call fork and print the parent and child process id.
Date: 26th Aug, 2025.
============================================================================
*/

#include <sys/types.h> // Import for `fork` & `getpid` system call
#include <unistd.h>    // Import for `fork` & `getpid` system call
#include <stdio.h>     // Import for printf function

int main(){
	if(!fork()){
		printf("Child PID: %d\n",getpid());
		printf("Parent PID: %d\n",getppid());
	}
	return 0;
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
Parent Process: PID = 45522, Child PID = 45523
Child Process: PID = 45523, Parent PID = 45522
*/