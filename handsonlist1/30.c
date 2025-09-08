/*
============================================================================
Name : 30.c
Author : Aman Kakadiya
Description : Write a program to run a script at a specific time using a Daemon process
Date: 27th Aug, 2025.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[])
{

     if (argc < 2){
        printf("Pass at least one argument\n");
        return 0;
    }
    time_t curr, dead;
    struct tm *deadline;
    time(&curr);
    deadline = localtime(&curr);

    deadline->tm_hour = atoi(argv[1]);
    deadline->tm_min = argv[2] == NULL ? 0 : atoi(argv[2]);
    deadline->tm_sec = argv[3] == NULL ? 0 : atoi(argv[3]);

    dead = mktime(deadline);

    int cid = fork();
    if (cid == 0)
    {
        setsid();
        do
        {
            time(&curr);
        } while (difftime(dead, curr) > 0);
        system("echo Current user=$USER");
    }
    else
    {
        exit(0);
    }

    return 0;
}
/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ cc 30.c
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out 19 28 46
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ Current user=amank
*/