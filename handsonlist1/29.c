/*
============================================================================
Name : 29.c
Author : Aman Kakadiya
Description : Write a program to get the scheduling policy and modify the scheduling policy (SCHED_FIFO, SCHED_RR).
Date: 27th Aug, 2025.
============================================================================
*/

#include <stdio.h>      // printf, perror
#include <unistd.h>     // getpid
#include <sys/types.h>  // pid_t
#include <sched.h>      // sched_getscheduler, sched_setscheduler, sched_param

int main() {
    int currentPolicy;
    pid_t pid;
    pid = getpid();
    currentPolicy = sched_getscheduler(pid);
    struct sched_param priority;
    priority.sched_priority = 10;

    switch (currentPolicy)
    {
    case SCHED_FIFO:
        printf("Current policy is FIFO\n");
        sched_setscheduler(pid, SCHED_RR, &priority);
        currentPolicy = sched_getscheduler(pid);
        printf("Current policy is %d\n", currentPolicy);
        break;
    case SCHED_RR:
        printf("Current policy is RR\n");
        sched_setscheduler(pid, SCHED_FIFO, &priority);
        currentPolicy = sched_getscheduler(pid);
        printf("Current policy is %d\n", currentPolicy);
        break;
    case SCHED_OTHER:
        printf("Current policy is OTHER\n");
        sched_setscheduler(pid, SCHED_RR, &priority);
        currentPolicy = sched_getscheduler(pid);
        printf("Current policy is %d\n", currentPolicy);
        break;
    default:
        perror("Error while getting current policy\n");
    }
    return 0;
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ sudo ./a.out
[sudo] password for amank: 
Current policy is OTHER
Current policy is 2
*/