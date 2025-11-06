/*
============================================================================
Name : 02.c
Author : Aman Kakadiya
Description :  Write a program to print the system resource limits. Use getrlimit system call.
Date: 20th Sep, 2025.
============================================================================
*/

#include <stdio.h>
#include <sys/resource.h>

int main()
{
	struct rlimit s;

	if(getrlimit(RLIMIT_AS, &s) != -1)
	{
		printf("Size of virtual memory\n");
		printf("Soft - %lu\n", s.rlim_cur);
		printf("Hard - %lu\n", s.rlim_max);
	}
	printf("\n");
	if (getrlimit(RLIMIT_CORE, &s) != -1)
	{
		printf("Size of core file\n");
		printf("Soft - %lu\n", s.rlim_cur);
		printf("Hard - %lu\n", s.rlim_max);
	}
	printf("\n");
	if (getrlimit(RLIMIT_CPU, &s) != -1)
	{
		printf("Limit on CPU time\n");
		printf("Soft - %lu\n", s.rlim_cur);
		printf("Hard - %lu\n", s.rlim_max);
	}
	printf("\n");
	if (getrlimit(RLIMIT_DATA, &s) != -1)
	{
		printf("Size of Data segment of process\n");
		printf("Soft - %lu\n", s.rlim_cur);
		printf("Hard - %lu\n", s.rlim_max);
	}
	printf("\n");
	if (getrlimit(RLIMIT_FSIZE, &s) != -1)
	{
		printf("Size of files that the process can create\n");
		printf("Soft - %lu\n", s.rlim_cur);
		printf("Hard - %lu\n", s.rlim_max);
	}
	printf("\n");
	if (getrlimit(RLIMIT_LOCKS, &s) != -1)
	{
		printf("Limit on the number of locks\n");
		printf("Soft - %lu\n", s.rlim_cur);
		printf("Hard - %lu\n", s.rlim_max);
	}
	printf("\n");
	if (getrlimit(RLIMIT_MEMLOCK, &s) != -1)
	{
		printf("Max no of bytes in memory that may be locked\n");
		printf("Soft - %lu\n", s.rlim_cur);
		printf("Hard - %lu\n", s.rlim_max);
	}
	printf("\n");
	if (getrlimit(RLIMIT_NICE, &s) != -1)
	{
		printf("Max nice value\n");
		printf("Soft - %lu\n", s.rlim_cur);
		printf("Hard - %lu\n", s.rlim_max);
	}
	printf("\n");
	if (getrlimit(RLIMIT_NOFILE, &s) != -1)
	{
		printf("Maximum no. file descriptors\n");
		printf("Soft - %lu\n", s.rlim_cur);
		printf("Hard - %lu\n", s.rlim_max);
	}
	printf("\n");
	if (getrlimit(RLIMIT_NPROC, &s) != -1)
	{
		printf("Number of extant processes for the real user id of the process\n");
		printf("Soft - %lu\n", s.rlim_cur);
		printf("Hard - %lu\n", s.rlim_max);
	}
	printf("\n");
	if (getrlimit(RLIMIT_RTPRIO, &s) != -1)
	{
		printf("Max real time priority\n");
		printf("Soft - %lu\n", s.rlim_cur);
		printf("Hard - %lu\n", s.rlim_max);
	}
	printf("\n");
	if (getrlimit(RLIMIT_RTTIME, &s) != -1)
	{
		printf("Max CPU time for a real time process\n");
		printf("Soft - %lu\n", s.rlim_cur);
		printf("Hard - %lu\n", s.rlim_max);
	}
	printf("\n");
	if (getrlimit(RLIMIT_SIGPENDING, &s) != -1)
	{
		printf("Max no of queued signals for the real user ID of the calling process\n");
		printf("Soft - %lu\n", s.rlim_cur);
		printf("Hard - %lu\n", s.rlim_max);
	}
	printf("\n");
	if (getrlimit(RLIMIT_STACK, &s) != -1)
	{
		printf("Max size of the process stack\n");
		printf("Soft - %lu\n", s.rlim_cur);
		printf("Hard - %lu\n", s.rlim_max);
	}

	return(0);
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058 $ ./a.out
Size of virtual memory
Soft - 18446744073709551615
Hard - 18446744073709551615

Size of core file
Soft - 0
Hard - 18446744073709551615

Limit on CPU time
Soft - 18446744073709551615
Hard - 18446744073709551615

Size of Data segment of process
Soft - 18446744073709551615
Hard - 18446744073709551615

Size of files that the process can create
Soft - 18446744073709551615
Hard - 18446744073709551615

Limit on the number of locks
Soft - 18446744073709551615
Hard - 18446744073709551615

Max no of bytes in memory that may be locked
Soft - 957353984
Hard - 957353984

Max nice value
Soft - 0
Hard - 0

Maximum no. file descriptors
Soft - 1073741816
Hard - 1073741816

Number of extant processes for the real user id of the process
Soft - 28489
Hard - 28489

Max real time priority
Soft - 0
Hard - 0

Max CPU time for a real time process
Soft - 18446744073709551615
Hard - 18446744073709551615

Max no of queued signals for the real user ID of the calling process
Soft - 28489
Hard - 28489

Max size of the process stack
Soft - 8388608
Hard - 18446744073709551615
*/