/*
============================================================================
Name : 09.c
Author : Aman Kakadiya
Description : Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal - Use signal system call.
Date: 20th Sep, 2025.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(){
    printf("Ignoring SIGINT signal\n");
    signal(SIGINT, SIG_IGN);
    // CTRL + C is ignored.
    sleep(7);
    printf("\nDefault action of SIGINT signal\n");
    signal(SIGINT, SIG_DFL);
    // CTRL + C is no longer ignored.
    sleep(7);
    return(0);
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058$ ./a.out 
Ignoring SIGINT signal
^C^C^C^C^C^C
Default action of SIGINT signal
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058$ ./a.out 
Ignoring SIGINT signal

Default action of SIGINT signal
^C
*/