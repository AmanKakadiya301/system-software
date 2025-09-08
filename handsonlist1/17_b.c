/*
============================================================================
Name : 17_b.c
Author : Aman Kakadiya
Description : Write a program to simulate online ticket reservations. 
		a .Implement a write lock.  Write a program to open a file, store a ticket number, and exit. 
		b. Write a separate program, to open the file,  implement write lock, read the ticket number, increment the number, and print the new ticket number then close the file
Date: 29th Aug, 2025.
============================================================================
*/

// Read and increment Ticket Number

#include <sys/types.h> // Import for `open`, `lseek`
#include <sys/stat.h>  // Import micros
#include <fcntl.h>     // Import for File control
#include <unistd.h>    // Import for `write`, `lseek` & `fcntl`
#include <stdio.h>     // Import for `perror` & `printf`
#include <string.h>     // For `strlen`
#define BUFFER_SIZE 1024

int main(){
	char *ticketFile = "./support_folder/ticket_file.txt";
	
	int fd,fileStatus;
	ssize_t readByte,writeByte;
	char buffer[BUFFER_SIZE];
	struct flock ticketLock;
	
	if((fd = open(ticketFile,O_RDWR | O_CREAT,S_IRWXU)) == -1){
		perror("Error while opening file\n");
		return 0;
	}

	ticketLock.l_type = F_WRLCK;
	ticketLock.l_pid = getpid();
	ticketLock.l_whence = SEEK_SET;
	ticketLock.l_len = 0;
	ticketLock.l_start = 0;

	if((fileStatus = fcntl(fd,F_SETLKW,&ticketLock)) == -1){
		perror("Error while applying lock\n");
		return 0;
	}
	
	int ticketNumber =0;
	lseek(fd,0,SEEK_SET);
	if((readByte = read(fd,buffer,sizeof(buffer)-1)) == -1){
		perror("Error while reading\n");
		return 0;
	}
	if(readByte>0){
		char *ptr = buffer;
		while(*ptr){
			ticketNumber = ticketNumber * 10 + (*ptr - '0');
			ptr++;
		}
	}

	ticketNumber++;

	lseek(fd, 0, SEEK_SET);

	sprintf(buffer, "%d", ticketNumber);

	if((writeByte = write(fd,buffer,strlen(buffer))) == -1){
		perror("Error while writing\n");
		return 0;
	}

	printf("New Ticket Number: %d\n",ticketNumber);


	ticketLock.l_type = F_UNLCK;
	if((fileStatus = fcntl(fd,F_SETLKW,&ticketLock)) == -1){
		perror("Error while releasing file\n");
	}
	return 0;
}


/*

amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ cc 17_b.c
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
New Ticket Number: 1
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
New Ticket Number: 2
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
New Ticket Number: 3
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
New Ticket Number: 4
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
New Ticket Number: 5
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
New Ticket Number: 6
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
New Ticket Number: 7
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
New Ticket Number: 8
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
New Ticket Number: 9
amank@amank-HP-EliteBook-x360-1030-G2:~/handsonlist1$ ./a.out
New Ticket Number: 10

*/