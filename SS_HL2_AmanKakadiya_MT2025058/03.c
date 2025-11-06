/*
============================================================================
Name : 03.c
Author : Aman Kakadiya
Description :  Write a program to set (any one) system resource limit. Use setrlimit system call.
Date: 20th Sep, 2025.
============================================================================
*/

#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>

// Use sudo
int main()
{
    struct rlimit rlim;
    printf("Old limits\n");
    getrlimit(RLIMIT_NICE, &rlim);

    printf("Soft - %lu\n", rlim.rlim_cur);
    printf("Hard - %lu\n", rlim.rlim_max);

    rlim.rlim_cur = 2;
    rlim.rlim_max = 3;

    if (setrlimit(RLIMIT_NICE, &rlim) != -1)
    {
        printf("New limits\n");
        getrlimit(RLIMIT_NICE, &rlim);

        printf("Soft - %lu\n", rlim.rlim_cur);
        printf("Hard - %lu\n", rlim.rlim_max);
    }
    else
    {
        perror("Error");
    }

    return(0);
}

/*

amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058 $ ./a.out
Old limits
Soft - 0
Hard - 0
Error: Operation not permitted
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058 $ sudo ./a.out
[sudo] password for amank: 
Old limits
Soft - 0
Hard - 0
New limits
Soft - 2
Hard - 3
*/