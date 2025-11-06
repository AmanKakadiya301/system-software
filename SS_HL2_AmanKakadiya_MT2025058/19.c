/*
============================================================================
Name : 19.c
Author : Aman Kakadiya
Description : Create a FIFO file by
                i. mknod command
                ii. mkfifo command
                iii. Use strace command to find out, which command (mknod or mkfifo) is better.
                iv. mknod system call
                v. mkfifo library function
Date: 20th Sep, 2025.
============================================================================
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

int main()
{
    char name[40];
    printf("Enter the name of fifo file: ");
    scanf("%s", name);

    int choice;
    printf("Choose an option:\n1.) mknod system call\n2.) mkfifo system call\n=> ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        printf("Using mknod system call...\n");

        if (mknod(name, __S_IFIFO | 0744, 0) == -1)
        {
            perror("Error: ");
            return -1;
        }
        printf("FIFO file created using mknod\n");
        break;
    case 2:
        printf("Using mkfifo system call...\n");

        if (mkfifo(name, 0744) == -1)
        {
            perror("Error: ");
            return -1;
        }
        printf("FIFO file created using mkfifo\n");
        break;

    default:
        printf("Invalid choice\n");
        break;
    }

    return (0);
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058 $ ./a.out
Enter the name of fifo file: FIFO
Choose an option:
1.) mknod system call
2.) mkfifo system call
=> 1
Using mknod system call...
FIFO file created using mknod
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058 $ ls -l
total 216
-rw-r--r-- 1 amank amank  1228 Sep 20 09:13 01_a.c
-rw-r--r-- 1 amank amank   997 Sep 20 09:13 01_b.c
-rw-r--r-- 1 amank amank   971 Sep 20 09:13 01_c.c
-rw-r--r-- 1 amank amank  4139 Sep 30 14:27 02.c
-rw-r--r-- 1 amank amank  1255 Sep 30 14:27 03.c
-rw-r--r-- 1 amank amank   848 Sep 30 14:28 04.c
-rw-r--r-- 1 amank amank  1982 Sep 30 14:36 05.c
-rw-r--r-- 1 amank amank   860 Sep 30 14:37 06.c
-rw-r--r-- 1 amank amank   939 Sep 30 15:59 07.c
-rw-r--r-- 1 amank amank  5402 Sep 30 15:59 08.c
-rw-r--r-- 1 amank amank   996 Sep 30 17:51 09.c
-rw-r--r-- 1 amank amank  2208 Sep 30 17:51 10.c
-rw-r--r-- 1 amank amank  1085 Sep 20 09:13 11.c
-rw-r--r-- 1 amank amank   938 Sep 30 18:13 12.c
-rw-r--r-- 1 amank amank  1170 Sep 30 18:13 13_receive.c
-rw-r--r-- 1 amank amank  1030 Sep 30 18:13 13_send.c
-rw-r--r-- 1 amank amank   998 Sep 30 18:14 14.c
-rw-r--r-- 1 amank amank  1024 Sep 30 18:15 15.c
-rw-r--r-- 1 amank amank  1186 Sep 30 18:16 16.c
-rw-r--r-- 1 amank amank  2088 Sep 30 18:18 17.c
-rw-r--r-- 1 amank amank  1407 Sep 20 09:13 18.c
-rw-r--r-- 1 amank amank  5199 Sep 20 09:13 19.c
-rw-r--r-- 1 amank amank  1553 Sep 20 09:13 20.c
-rw-r--r-- 1 amank amank  1956 Sep 20 09:13 21.c
-rw-r--r-- 1 amank amank  1329 Sep 20 09:13 22.c
-rw-r--r-- 1 amank amank  1094 Sep 20 09:13 22_write.c
-rw-r--r-- 1 amank amank   815 Sep 20 09:13 23.c
-rw-r--r-- 1 amank amank   689 Sep 20 09:13 24.c
-rw-r--r-- 1 amank amank  2056 Sep 20 09:13 25.c
-rw-r--r-- 1 amank amank  1656 Sep 20 09:13 26.c
-rw-r--r-- 1 amank amank  2313 Sep 20 09:13 27.c
-rw-r--r-- 1 amank amank  1216 Sep 20 09:13 28.c
-rw-r--r-- 1 amank amank   703 Sep 20 09:13 29.c
-rw-r--r-- 1 amank amank   693 Sep 20 09:13 30_a.c
-rw-r--r-- 1 amank amank  1145 Sep 20 09:13 30_b.c
-rw-r--r-- 1 amank amank   709 Sep 20 09:13 30_c.c
-rw-r--r-- 1 amank amank   950 Sep 20 09:13 30_d.c
-rw-r--r-- 1 amank amank  2206 Sep 20 09:13 31.c
-rw-r--r-- 1 amank amank  1431 Sep 20 09:13 32_a.c
-rw-r--r-- 1 amank amank  2315 Sep 20 09:13 32_b.c
-rw-r--r-- 1 amank amank  1714 Sep 20 09:13 32_c.c
-rw-r--r-- 1 amank amank  2627 Sep 20 09:13 32_d.c
-rw-r--r-- 1 amank amank  1249 Sep 20 09:13 33_client.c
-rw-r--r-- 1 amank amank  1188 Sep 20 09:13 33_server.c
-rw-r--r-- 1 amank amank  3318 Sep 20 09:13 34_a.c
-rw-r--r-- 1 amank amank  3644 Sep 20 09:13 34_b.c
-rw-r--r-- 1 amank amank  1468 Sep 20 09:13 34_client.c
-rwxrwxr-x 1 amank amank 16296 Sep 30 18:20 a.out
prwxr--r-- 1 amank amank     0 Sep 30 18:21 FIFO
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058 $ ./a.out
Enter the name of fifo file: FIFO2
Choose an option:
1.) mknod system call
2.) mkfifo system call
=> 2
Using mkfifo system call...
FIFO file created using mkfifo
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058 $ ls -l
total 216
-rw-r--r-- 1 amank amank  1228 Sep 20 09:13 01_a.c
-rw-r--r-- 1 amank amank   997 Sep 20 09:13 01_b.c
-rw-r--r-- 1 amank amank   971 Sep 20 09:13 01_c.c
-rw-r--r-- 1 amank amank  4139 Sep 30 14:27 02.c
-rw-r--r-- 1 amank amank  1255 Sep 30 14:27 03.c
-rw-r--r-- 1 amank amank   848 Sep 30 14:28 04.c
-rw-r--r-- 1 amank amank  1982 Sep 30 14:36 05.c
-rw-r--r-- 1 amank amank   860 Sep 30 14:37 06.c
-rw-r--r-- 1 amank amank   939 Sep 30 15:59 07.c
-rw-r--r-- 1 amank amank  5402 Sep 30 15:59 08.c
-rw-r--r-- 1 amank amank   996 Sep 30 17:51 09.c
-rw-r--r-- 1 amank amank  2208 Sep 30 17:51 10.c
-rw-r--r-- 1 amank amank  1085 Sep 20 09:13 11.c
-rw-r--r-- 1 amank amank   938 Sep 30 18:13 12.c
-rw-r--r-- 1 amank amank  1170 Sep 30 18:13 13_receive.c
-rw-r--r-- 1 amank amank  1030 Sep 30 18:13 13_send.c
-rw-r--r-- 1 amank amank   998 Sep 30 18:14 14.c
-rw-r--r-- 1 amank amank  1024 Sep 30 18:15 15.c
-rw-r--r-- 1 amank amank  1186 Sep 30 18:16 16.c
-rw-r--r-- 1 amank amank  2088 Sep 30 18:18 17.c
-rw-r--r-- 1 amank amank  1407 Sep 20 09:13 18.c
-rw-r--r-- 1 amank amank  5199 Sep 20 09:13 19.c
-rw-r--r-- 1 amank amank  1553 Sep 20 09:13 20.c
-rw-r--r-- 1 amank amank  1956 Sep 20 09:13 21.c
-rw-r--r-- 1 amank amank  1329 Sep 20 09:13 22.c
-rw-r--r-- 1 amank amank  1094 Sep 20 09:13 22_write.c
-rw-r--r-- 1 amank amank   815 Sep 20 09:13 23.c
-rw-r--r-- 1 amank amank   689 Sep 20 09:13 24.c
-rw-r--r-- 1 amank amank  2056 Sep 20 09:13 25.c
-rw-r--r-- 1 amank amank  1656 Sep 20 09:13 26.c
-rw-r--r-- 1 amank amank  2313 Sep 20 09:13 27.c
-rw-r--r-- 1 amank amank  1216 Sep 20 09:13 28.c
-rw-r--r-- 1 amank amank   703 Sep 20 09:13 29.c
-rw-r--r-- 1 amank amank   693 Sep 20 09:13 30_a.c
-rw-r--r-- 1 amank amank  1145 Sep 20 09:13 30_b.c
-rw-r--r-- 1 amank amank   709 Sep 20 09:13 30_c.c
-rw-r--r-- 1 amank amank   950 Sep 20 09:13 30_d.c
-rw-r--r-- 1 amank amank  2206 Sep 20 09:13 31.c
-rw-r--r-- 1 amank amank  1431 Sep 20 09:13 32_a.c
-rw-r--r-- 1 amank amank  2315 Sep 20 09:13 32_b.c
-rw-r--r-- 1 amank amank  1714 Sep 20 09:13 32_c.c
-rw-r--r-- 1 amank amank  2627 Sep 20 09:13 32_d.c
-rw-r--r-- 1 amank amank  1249 Sep 20 09:13 33_client.c
-rw-r--r-- 1 amank amank  1188 Sep 20 09:13 33_server.c
-rw-r--r-- 1 amank amank  3318 Sep 20 09:13 34_a.c
-rw-r--r-- 1 amank amank  3644 Sep 20 09:13 34_b.c
-rw-r--r-- 1 amank amank  1468 Sep 20 09:13 34_client.c
-rwxrwxr-x 1 amank amank 16296 Sep 30 18:20 a.out
prwxr--r-- 1 amank amank     0 Sep 30 18:21 FIFO
prwxr--r-- 1 amank amank     0 Sep 30 18:22 FIFO2
*/