/*
============================================================================
Name : 23.c
Author : Aman Kakadiya
Description : Write a program to print the maximum number of files can be opened within a process and size of a pipe (circular buffer).
Date: 20th Sep, 2025.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>

int main(){
    long PIPE_BUF, OPEN_MAX;
    
    PIPE_BUF = pathconf(".", _PC_PIPE_BUF);
    OPEN_MAX = sysconf(_SC_OPEN_MAX);

    printf("Pipe_buf = %ld\nOpen_max = %ld\n", PIPE_BUF, OPEN_MAX);
    
    return(0);
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058 $ ./a.out
Pipe_buf = 4096
Open_max = 1073741816
*/