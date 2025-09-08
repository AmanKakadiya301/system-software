/*
============================================================================
Name : 24.c
Author : Aman Kakadiya
Description : Write a program to create an orphan process
Date: 26th Aug, 2025.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child branch
        printf("Child process: Initial PPID = %d\n", getppid());
        sleep(1); // give parent time to exit
        printf("Child process: New PPID after orphan = %d\n", getppid());
    } 
    else if (pid > 0) {
        // Parent branch
        printf("Parent process running, PID = %d\n", getpid());
        _exit(0);  // terminate parent immediately
    } 
    else {
        perror("fork failed");
        return 1;
    }
    return 0;
} 

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ cc 24.c
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
Parent process running, PID = 47844
Child process: Initial PPID = 47844
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ Child process: New PPID after orphan = 3183

*/