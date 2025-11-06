/*
============================================================================
Name : 21.c
Author : Aman Kakadiya
Description : Write two programs so that both can communicate by FIFO - Use two way communications.
Date: 20th Sep, 2025.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
    // int res = mkfifo("myFifo", S_IFIFO | 0666);
    // int res = mkfifo("yourFifo", S_IFIFO | 0666);

    int choice = 0;
    printf("1.\t read/write (program 1)\n2.\t read/write (program 2)\n\t Enter choice: ");
    scanf("%d", &choice);
    int fd1 = -1;
    int fd2 = -1;
    char buf[100];

    // Note that read/write are blocking calls.

    switch (choice)
    {
    case 1:
        fd1 = open("text.txt", O_WRONLY, 0666);
        write(fd1, "Hello from 1 to 2\n", sizeof("Hello from 1 to 2\n"));
        close(fd1);

        fd2 = open("FIFO", O_RDONLY, 0666);
        read(fd2, buf, sizeof(buf));
        printf("Message from 2 to 1: %s", buf);

        close(fd2);
        break;
    case 2:
        fd2 = open("text.txt", O_RDONLY, 0666);
        read(fd2, buf, sizeof(buf));
        printf("Message from writer: %s", buf);
        close(fd2);

        fd1 = open("FIFO", O_WRONLY, 0666);
        write(fd1, "Hello from 2 to 1\n", sizeof("Hello from 2 to 1\n"));
        close(fd1);

        break;
    default:
        printf("Choose a valid choice\n");
        break;
    }

    return (0);
}

/*

amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058$ ./a.out 
1.	 read/write (program 1)
2.	 read/write (program 2)
	 Enter choice: 1
Message from 2 to 1: Hello from 2 to 1


amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058$ ./a.out 
1.	 read/write (program 1)
2.	 read/write (program 2)
	 Enter choice: 2
Message from writer: Hello from 1 to 2
*/