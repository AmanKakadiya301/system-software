/*
============================================================================
Name : 32_a.c
Author : Aman Kakadiya
Description : Write a program to implement semaphore to protect any critical section.
                a. Rewrite the ticket number creation program using semaphore
                b. Protect shared memory from concurrent write access
                c. Protect multiple pseudo resources (may be two) using counting semaphore
                d. Remove the created semaphore
Date: 20th Sep, 2025.
============================================================================
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>

struct ticket
{
    char *train_name;
    int ticket_num;
};

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main()
{

    int fd = open("ticket_record.txt", O_RDWR | O_CREAT, 0744);
    struct ticket ticket1 = {"AMAN", 100};

    write(fd, &ticket1, sizeof(struct ticket));

    printf("Train name: %s\n", ticket1.train_name);
    printf("Ticket number: %d\n", ticket1.ticket_num);

    close(fd);

    int key = ftok(".", 'a');
    int sem_id = semget(key, 1, IPC_CREAT | 0666);

    union semun arg;
    arg.val = 1;
    semctl(sem_id, 0, SETVAL, arg);

    return 0;
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058 $ ./a.out
Train name: AMAN
Ticket number: 100

*/