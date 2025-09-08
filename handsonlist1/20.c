/*
============================================================================
Name : 20.c
Author : Aman Kakadiya
Description : Find out the priority of your running program. Modify the priority with nice command.
Date: 26th Aug, 2025.
============================================================================
*/

#include <unistd.h> // Import for `nice` system call
#include <stdio.h>  // Import for `printf` function
#include <stdlib.h> // `atoi` conversion from string to int

int main(int argc, char *argv[]){
	if(argc < 2){
		perror("Enter New Priority Number\n");
		return 0;
	}
	int newPriority = atoi(argv[1]);
	int priority = nice(0);
	printf("Current Priority: %d\n",priority);
	priority = nice(newPriority);
	printf("New Priority: %d\n",priority);
	return 0;
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ cc 20.c
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out 2
Current Priority: 0
New Priority: 2
*/