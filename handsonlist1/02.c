/*
============================================================================
Name : 02.c
Author : Aman Kakadiya
Description : Write a simple program to execute in an infinite loop at the background. Go to /proc directory and identify all the process related information in the corresponding proc directory.
Date: 22nd Aug, 2025.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>

int main() {
    while (1) {
        printf("Running in background... PID: %d\n", getpid());
        sleep(5); // Sleep for 5 seconds to avoid CPU hogging
    }
    return 0;
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out 02.c
Running in background... PID: 32539
*/