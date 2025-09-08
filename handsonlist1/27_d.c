/*
============================================================================
Name : 27_d.c
Author : Aman Kakadiya
Description : Write a program to execute `ls -Rl` by the following system calls 
    d. `execv` 
Date: 26th Aug, 2025.
============================================================================
*/

#include <unistd.h> // Import for `execv` function
#include <stdio.h>  // Import for `printf` function

int main(){
	char *args[] = {"/bin/ls","-Rl",NULL};
	
	printf("===== Output using execv =====\n");
	execv(args[0],args);
	printf("\n");
	return 0;
}


/*amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
===== Output using execv =====
.:
total 228
-rw-r--r-- 1 amank amank   872 Sep  7 11:38 01_a.c
-rw-r--r-- 1 amank amank   846 Sep  7 12:18 01_b.c
-rw-r--r-- 1 amank amank  1322 Sep  7 01:25 01_c.c
-rwxrwxr-x 1 amank amank   246 Sep  7 11:35 01.sh
-rw-rw-r-- 1 amank amank   249 Sep  7 14:54 01_shell.sh
-rw-r--r-- 1 amank amank   684 Sep  7 12:18 02.c
-rw-r--r-- 1 amank amank  1054 Sep  7 12:18 03.c
-rw-r--r-- 1 amank amank  1239 Sep  7 12:02 04.c
-rw-r--r-- 1 amank amank  1062 Sep  7 12:18 05.c
-rw-r--r-- 1 amank amank   811 Sep  7 12:18 06.c
-rw-r--r-- 1 amank amank  2492 Sep  7 12:18 07.c
-rw-r--r-- 1 amank amank  2126 Sep  7 12:18 08.c
-rw-r--r-- 1 amank amank  2038 Sep  7 14:17 09.c
-rw-r--r-- 1 amank amank  1475 Sep  7 14:23 10.c
-rw-r--r-- 1 amank amank  1457 Sep  7 14:29 11_a.c
-rw-r--r-- 1 amank amank  1538 Sep  7 14:31 11_b.c
-rw-r--r-- 1 amank amank  1484 Sep  7 14:33 11_c.c
-rw-r--r-- 1 amank amank  2838 Sep  7 14:37 12.c
-rw-r--r-- 1 amank amank  1327 Sep  7 14:39 13.c
-rw-r--r-- 1 amank amank  1692 Sep  7 14:42 14.c
-rw-r--r-- 1 amank amank  4320 Sep  7 14:42 15.c
-rw-r--r-- 1 amank amank  1871 Sep  7 15:14 16_a.c
-rw-r--r-- 1 amank amank  1880 Sep  7 15:14 16_b.c
-rw-r--r-- 1 amank amank  2137 Sep  7 15:19 17_a.c
-rw-r--r-- 1 amank amank  3043 Sep  7 15:19 17_b.c
-rw-r--r-- 1 amank amank  2872 Sep  7 15:21 18.c
-rw-r--r-- 1 amank amank  1064 Sep  7 15:28 19.c
-rw-r--r-- 1 amank amank   944 Sep  7 15:29 20.c
-rw-r--r-- 1 amank amank   772 Sep  7 15:56 21.c
-rw-r--r-- 1 amank amank  1163 Sep  7 16:00 22.c
-rw-r--r-- 1 amank amank  1337 Sep  7 16:22 23.c
-rw-r--r-- 1 amank amank  1184 Sep  7 16:39 24.c
-rw-r--r-- 1 amank amank  1989 Sep  7 16:40 25.c
-rw-r--r-- 1 amank amank   790 Sep  7 16:44 26.c
-rw-r--r-- 1 amank amank  3823 Sep  7 16:47 27_a.c
-rw-r--r-- 1 amank amank  3827 Sep  7 16:48 27_b.c
-rw-r--r-- 1 amank amank  3831 Sep  7 16:50 27_c.c
-rw-r--r-- 1 amank amank  3392 Sep  6 19:33 27_d.c
-rw-r--r-- 1 amank amank  3396 Sep  6 19:33 27_e.c
-rw-r--r-- 1 amank amank   842 Sep  6 19:33 28.c
-rw-r--r-- 1 amank amank  1467 Sep  6 19:33 29.c
-rw-r--r-- 1 amank amank  4303 Sep  7 01:21 30.c
-rwxrwxr-x 1 amank amank   303 Sep  7 10:51 30.sh
-rwxrwxr-x 1 amank amank 16168 Sep  7 16:50 a.out
-rw-rw-r-- 1 amank amank    13 Sep  7 16:00 b.txt
-rw-rw-r-- 2 amank amank    31 Sep  7 11:51 hardLink.txt
prwx------ 1 amank amank     0 Sep  7 11:57 mkfifo
prwx------ 1 amank amank     0 Sep  7 11:57 mknod
-rwx------ 1 amank amank     0 Sep  7 11:59 newFile
-rwxrwxr-x 1 amank amank 16208 Sep  7 16:43 Q14
lrwxrwxrwx 1 amank amank    10 Sep  7 11:53 softLink.txt -> source.txt
-rw-rw-r-- 2 amank amank    31 Sep  7 11:51 source.txt
drwxrwxr-x 2 amank amank  4096 Sep  7 01:03 support_folder
-rwx------ 1 amank amank     0 Sep  7 12:10 test1.txt
-rwx------ 1 amank amank     0 Sep  7 12:10 test2.txt
-rwx------ 1 amank amank     0 Sep  7 12:10 test3.txt
-rwx------ 1 amank amank     0 Sep  7 12:10 test4.txt
-rw-rw-r-- 1 amank amank     0 Sep  7 14:36 test.txt

./support_folder:
total 8
-rw-r--r-- 1 amank amank 24 Sep  7 15:21 18-file.txt
-rw-r--r-- 1 amank amank  2 Sep  7 15:18 ticket_file.txt
*/