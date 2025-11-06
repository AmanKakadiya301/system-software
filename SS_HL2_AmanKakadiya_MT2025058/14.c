/*
============================================================================
Name : 14.c
Author : Aman Kakadiya
Description : Write a simple program to create a pipe, write to the pipe, read from pipe and display on the monitor.
Date: 20th Sep, 2025.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd[2];
    
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }
    printf("pipe created with fd1: %d, fd2: %d\n", fd[0], fd[1]);

    char buf[10];
    
    if (write(fd[1], "Hello\n", sizeof("Hello\n")) == -1) {
        perror("write");
        return 1;
    }
    
    if (read(fd[0], buf, sizeof(buf)) == -1) {
        perror("read");
        return 1;
    }

    printf("Buffer: %s", buf);

    getchar();

    return 0;
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058$ ./a.out 
pipe created with fd1: 3, fd2: 4
Buffer: Hello
*/