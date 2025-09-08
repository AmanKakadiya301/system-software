/*
============================================================================
Name : 23.c
Author : Aman Kakadiya
Description : Write a program to create a Zombie state of the running program
Date: 26th Aug, 2025.
============================================================================
*/

#include <sys/types.h> // Import for `fork` system call
#include <sys/wait.h> //wait
#include <unistd.h>    // Import for `fork` system call
#include <stdio.h>     // Import for `printf` function
#include <stdlib.h>	   // Import `wait` to cleanup zombie state

int main(){
	pid_t pid= fork();

	if(pid>0){
		printf("Parent PID: %d\n",getpid());

		printf("Putting the parent to sleep for 10s!\n");
        sleep(10);
        printf("Parent is now awake!\n");

		wait(NULL);
		printf("Child reaped, zombie cleared.\n");
	}
	else if(pid==0){
		printf("Child PID: %d\n",getpid());
		printf("Putting the child to sleep for 5s!\n");
        sleep(5);
		printf("Child is exiting now\n");
		_exit(0);
	}
	else{
		perror("fork failed");
		printf("Child is exiting now\n");

		return 1;
	}
	return 0;
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
Parent PID: 46581
Putting the parent to sleep for 10s!
Child PID: 46582
Putting the child to sleep for 5s!
Child is exiting now
Parent is now awake!
Child reaped, zombie cleared.
*/