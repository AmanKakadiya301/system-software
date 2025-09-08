/*
============================================================================
Name : 25.c
Author : Aman Kakadiya
Description : Write a program to create three child processes. The parent should wait for a particular child (use `waitpid` system call).
Date: 26th Aug, 2025.
============================================================================
*/

#include <unistd.h>    // Import for `fork`
#include <sys/types.h> // Import for `fork`, `waitpid`
#include <sys/wait.h>  // Import for `waitpid`
#include <stdio.h>     // Import for `printf`

int main(){
	pid_t childA,childB,childC;
	pid_t terminatedChild;
	int waitStatus;

	if((childA = fork()) == -1){
		perror("Error while Creating child A\n");
		return 0;
	}
	if(childA == 0){
		printf("Child A created !!!\n");
		printf("Putting Child A to sleep for 5 secs\n");
		sleep(5);
		printf("Child A Woke up\n");
		_exit(0);
	}

	if((childB = fork()) == -1){
		perror("Error while Creating child B\n");
		return 0;
	}
	if(childB == 0){
		printf("Child B Created !!\n");
		printf("Putting Child B to sleep for 10 secs\n");
		sleep(10);
		printf("Child B Woke up\n");
		_exit(0);
	}

	if((childC = fork()) == -1){
		perror("Error while creating child C\n");
		return 0;
	}
	if(childC == 0){
		printf("Child C Created !!\n");
		printf("Putting Child C to sleep for 15 secs\n");
		sleep(15);
		printf("Child C Woke up\n");
		_exit(0);
	}

	printf("Parent waiting for Child C\n");
	terminatedChild = waitpid(childC,&waitStatus,0);
	
	if(terminatedChild == childC){
		printf("Child C has exited with exit status: %d\n",waitStatus);
	}else{
		perror("Some Error Occurred\n");
	}
	return 0;
}
/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
Child A created !!!
Putting Child A to sleep for 5 secs
Parent waiting for Child C
Child B Created !!
Putting Child B to sleep for 10 secs
Child C Created !!
Putting Child C to sleep for 15 secs
Child A Woke up
Child B Woke up
Child C Woke up
Child C has exited with exit status: 0
*/