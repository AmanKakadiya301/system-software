/*
============================================================================
Name : 04.c
Author : Aman Kakadiya
Description :  Write a program to measure how much time is taken to execute 100 getppid() system call. Use time stamp counter.
Date: 20th Sep, 2025.
============================================================================
*/

#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main() {
    clock_t start, end;
    start = clock();

    for (int i = 0; i < 100; i++) {
        getppid();
    }

    end = clock();
    double duration = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Time taken for 100 getppid system calls: %lf seconds\n", duration);
    return 0;
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058 $ ./a.out
Time taken for 100 getppid system calls: 0.000265 seconds
*/