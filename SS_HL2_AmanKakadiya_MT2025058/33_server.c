/*
============================================================================
Name : 33_server.c
Author : Aman Kakadiya
Description : Write a program to communicate between two machines using socket.
Date: 20th Sep, 2025.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
    struct sockaddr_in serv, cli;
    int sd;
    sd = socket(AF_INET, SOCK_STREAM, 0);
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(5000);
    
    bind(sd, (struct sockaddr *)&serv, sizeof(serv));
    listen(sd, 5);

    int nsd;

    int len = sizeof(cli);
    nsd = accept(sd, (struct sockaddr *)&cli, &len);

    char buf[100];
    int ret;
    ret = read(nsd, buf, sizeof(buf));
    write(1, buf, ret);
    write(nsd, "Message From server\n", sizeof("Message From server\n"));

    close(sd);

    return(0);
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058$ ./client 
Sending message to server...
Sent!
Press return key to read...

Message From server
*/