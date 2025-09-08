/*
============================================================================
Name : 16_b.c
Author : Aman Kakadiya
Description : Write a program to perform mandatory locking. 
    - Implement read lock 
Date: 26th Aug, 2025.
============================================================================
*/

#include <stdio.h>		// Import `printf` and `perror`
#include <fcntl.h>   	// Import file control
#include <sys/stat.h> 	// Import system call macros
#include <sys/types.h> 	// Import system call macros
#include <unistd.h>  	// Import system call macros

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Enter File Name\n");
        return 1;
    }

    struct flock lock, lockStatus;

    // Defining Lock Mechanism
    lock.l_type = F_RDLCK;       // Read Lock
    lock.l_whence = SEEK_SET;    // Seek to the start of the file
    lock.l_start = 0;            // Lock from the start of the file
    lock.l_len = 0;              // Locks the entire file
    lock.l_pid = getpid();       // Process ID (not used in setting the lock)

    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        perror("Error while opening the file\n");
        return 0;
    }

    // Check the current lock status
    if (fcntl(fd, F_GETLK, &lockStatus) == -1) {
        perror("Error while checking the file lock status\n");
        return 0;
    }

    if (lockStatus.l_type == F_WRLCK) {
        printf("File has Write Lock\n");
    } else if (lockStatus.l_type == F_RDLCK) {
        printf("File has Read Lock\n");
    } else {
        if (fcntl(fd, F_SETLK, &lock) == -1) {
            perror("Error while setting the read lock\n");
            return 0;
        }
        printf("File Lock has been applied\n");
        //pause();
    }
    return 0;
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out test.txt
File Lock has been applied
*/