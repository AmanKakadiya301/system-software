/*
============================================================================
Name : 19.c
Author : Aman Kakadiya
Description : Write a program to find out time taken to execute getpid system call. Use time stamp counter.
Date: 26th Aug, 2025.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

static __inline__ uint64_t rdtsc(void) {
    unsigned int hi, lo;
    __asm__ volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

int main() {
    uint64_t start, end, cycles;
    pid_t pid;
    double cpu_freq_ghz = 3.0;  // Set your CPU frequency in GHz here

    start = rdtsc();
    pid = getpid();
    end = rdtsc();

    cycles = end - start;
    // Convert cycles to nanoseconds
    double time_ns = (double)cycles / cpu_freq_ghz;

    printf("PID: %d\n", pid);
    printf("Time Taken: %.0f ns\n", time_ns);

    return 0;
}


/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
PID: 43905
Time Taken: 41 ns
*/
