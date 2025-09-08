/*
============================================================================
Name : 16_a.c
Author : Aman Kakadiya
Description : Write a program to perform mandatory locking. 
    - Implement write lock 
Date: 26th Aug, 2025.
============================================================================
*/#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Enter File Name\n");
        return 0;
    }

    struct flock lock;
    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        perror("Error while opening the file\n");
        return 0;
    }

    // Set up for lock status check
    lock.l_type = F_WRLCK;       // Requesting write lock
    lock.l_whence = SEEK_SET;    // Relative to start of file
    lock.l_start = 0;            // Start offset
    lock.l_len = 0;              // Lock whole file
    lock.l_pid = getpid();

    // Check lock status
    if (fcntl(fd, F_GETLK, &lock) == -1) {
        perror("Error getting lock status\n");
        close(fd);
        return 0;
    }

    if (lock.l_type == F_UNLCK) {
        // Lock is available, apply it
        lock.l_type = F_WRLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1) {
            perror("Could not set write lock\n");
        } else {
            printf("File Lock has been applied\n");
            pause(); // Wait until killed
        }
    } else if (lock.l_type == F_WRLCK) {
        printf("File has Write Lock\n");
    } else if (lock.l_type == F_RDLCK) {
        printf("File has Read Lock\n");
    }

    close(fd);
    return 0;
}



/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out test.txt
File Lock has been applied
*/

//Other terminal

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out test.txt
File has Write Lock
*/