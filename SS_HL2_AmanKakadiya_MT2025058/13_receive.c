/*
============================================================================
Name : 13.c
Author : Aman Kakadiya
Description : Two programs: first program is waiting to catch SIGSTOP signal, the second program
              will send the signal (using kill system call). Find out whether the first program is able to catch
              the signal or not. (First).
Date: 20th Sep, 2025.
============================================================================
*/

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <bits/sigaction.h>

void catch (int sig)
{
    printf("Signal Caught - %d\n", sig);
    exit(0);
}

int main()
{
    printf("pid (reciever): %d\n", getpid());

    struct sigaction ac;
    memset(&ac, 0, sizeof(ac));
    
    // The handler will be ignored, as SIGSTOP and SIGKILL can not be caught.
    ac.sa_handler = catch;
    for (;;)
    {
        sigaction(SIGSTOP, &ac, NULL);
    }
    return (0);
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058 $ cc 13_receive.c -o 13r
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058 $ ./13r
pid (reciever): 75661

[1]+  Stopped                 ./13r

*/