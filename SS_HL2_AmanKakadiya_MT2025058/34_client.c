/*
============================================================================
Name : 34_client.c
Author : Aman Kakadiya
Description : Helper for question 34
Date: 20th Sep, 2025.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main() {
    int socket_fd;
    struct sockaddr_in server_address;
    char buffer[MAX_BUFFER_SIZE] = {0};

    // Create socket
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Configure server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5555);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        return -1;
    }

    // Send message to the server
    char *message = "Hello from client";
    send(socket_fd, message, strlen(message), 0);
    printf("Message sent to server: %s\n", message);

    // Receive message from the server
    int read_size = read(socket_fd, buffer, MAX_BUFFER_SIZE);
    if (read_size > 0) {
        printf("Message from server: %s\n", buffer);
    }

    // Close socket
    close(socket_fd);
    return 0;
}
