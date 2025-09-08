/*
============================================================================
Name : 14.c
Author : Aman Kakadiya
Description : Write a program to find the type of a file. 
    1. Input should be taken from command line. 
    2. Program should be able to identify any type of a file.
Date: 25th Aug, 2025.
============================================================================
*/

#include <sys/types.h> // Imports for using `stat` system call
#include <sys/stat.h>  // Imports for using `stat` system call
#include <unistd.h>    // Imports for using `stat` system call
#include <fcntl.h>     // Import File Control
#include <stdio.h>     // Import for `printf`

int main(int argc, char *argv[])
{
    if (argc < 2){
        printf("Pass the name of the file as the argument");
        return 0;
    }
    
    int fd = open(argv[1], O_RDONLY);
    struct stat statbuf;

    fstat(fd, &statbuf);

    if (S_ISREG(statbuf.st_mode))
        write(STDOUT_FILENO, "Regular File\n", 13);
    else if (S_ISDIR(statbuf.st_mode))
        write(STDOUT_FILENO, "Directory File\n", 15);
    else if (S_ISCHR(statbuf.st_mode))
        write(STDOUT_FILENO, "Character File\n", 15);
    else if (S_ISBLK(statbuf.st_mode))
        write(STDOUT_FILENO, "Block File\n", 11);
    else if (S_ISFIFO(statbuf.st_mode))
        write(STDOUT_FILENO, "FIFO File\n", 10);
    else if (S_ISLNK(statbuf.st_mode))
        write(STDOUT_FILENO, "Symbolic Link File\n", 19);
    else if (S_ISSOCK(statbuf.st_mode))
        write(STDOUT_FILENO, "Socket\n", 7);
    else
        write(STDOUT_FILENO, "Error\n", 6);
    return 0;
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out 14.c~
Regular File
*/