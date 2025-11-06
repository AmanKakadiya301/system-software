/*
============================================================================
Name : 30_b.c
Author : Aman Kakadiya
Description : Write a program to create a shared memory.
                b. Attach with O_RDONLY and check whether you are able to overwrite.
Date: 20th Sep, 2025.
============================================================================
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    key_t key = ftok(".", 'a');
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    int shmid = shmget(key, 1024, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    char *data = shmat(shmid, NULL, 0);
    if (data == (char *)(-1)) {
        perror("shmat");
        return 1;
    }

    printf("Enter text to put in shared memory: ");
    scanf("%[^\n]s", data);

    printf("Write successful: %s\n", data);
    shmdt(data);

    return 0;
}
/*
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058 $ ./a.out
Enter text to put in shared memory: hru ?
Write successful: hru ?
*/