/*
============================================================================
Name : 17.c
Author : Aman Kakadiya
Description : Write a program to execute ls -l | wc
                a. Use dup
                b. Use dup2
                c. Use fcntl
Date: 20th Sep, 2025.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd[2];
	pipe(fd);

	int choice = 0;
	printf("1.\t Using dup\n2.\t Using dup2\n3.\t Using fcntl\n\t Enter choice: ");
	scanf("%d", &choice);
	switch (choice)
	{
	case 1:
		// (a.)
		if (!fork())
		{
			int c_value = 101;
			close(fd[0]);
			close(1);
			dup(fd[1]);
			execl("/bin/ls", "/bin/ls", "-l", NULL);
		}
		else
		{
			int p_value;
			close(0);
			close(fd[1]);
			dup(fd[0]);
			execl("/bin/wc", "/bin/wc", NULL);
		}
		break;
	case 2:
		// (b.)
		if (!fork())
		{
			int c_value = 101;
			close(fd[0]);
			close(1);
			dup2(fd[1], 1);
			execl("/bin/ls", "/bin/ls", "-l", NULL);
		}
		else
		{
			int p_value;
			close(0);
			close(fd[1]);
			dup2(fd[0], 0);
			execl("/bin/wc", "/bin/wc", NULL);
		}
		break;
	case 3:
		// (c.)
		if (!fork())
		{
			int c_value = 101;
			close(fd[0]);
			close(1);
			fcntl(fd[1], F_DUPFD, 1);
			execl("/bin/ls", "/bin/ls", "-l", NULL);
		}
		else
		{
			int p_value;
			close(0);
			close(fd[1]);
			fcntl(fd[0], F_DUPFD, 0);
			execl("/bin/wc", "/bin/wc", NULL);
		}
		break;
	default:
		break;
	}

	return (0);
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058 $ ./a.out
1.       Using dup
2.       Using dup2
3.       Using fcntl
         Enter choice: 1
     49     434    2429
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058 $ ./a.out
1.       Using dup
2.       Using dup2
3.       Using fcntl
         Enter choice: 2
     49     434    2429
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058 $ ./a.out
1.       Using dup
2.       Using dup2
3.       Using fcntl
         Enter choice: 3
     49     434    2429
*/