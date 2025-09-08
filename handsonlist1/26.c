/*
============================================================================
Name : 26.c
Author : Aman Kakadiya
Description : Write a program to execute an executable program. 
    1. Use some executable program 
    2. Pass some input to an executable program. (for example execute an executable of `$./a.out name`)
Date: 26th Aug, 2025.
============================================================================
*/


#include <unistd.h>	// Import execl

int main(){
	char *exec_path = "./Q14";
	char *argv = "10.c";
	execl(exec_path,exec_path,argv,NULL);

	return 0;
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ cc 14.c -o Q14
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ cc 26.c
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
Regular Files
*/
