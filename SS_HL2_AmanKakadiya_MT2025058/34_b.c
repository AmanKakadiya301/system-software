/*
============================================================================
Name : 34_b.c
Author : Aman Kakadiya
Description : Write a program to create a concurrent server.
                a. Use fork
                b. Use pthread_create
Date: 20th Sep, 2025.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 5555
#define BUFFER_SIZE 1024

void *handle_client(void *socket_desc) {
    int nsfd = *(int *)socket_desc;
    free(socket_desc);  // Free the allocated memory for the socket descriptor

    char buffer[BUFFER_SIZE] = {0};
    int read_size;

    // Receive messages from the client
    while ((read_size = read(nsfd, buffer, sizeof(buffer))) > 0) {
        printf("Received from client: %s\n", buffer);

        // Send a response back to the client
        const char *message = "Message From server\n";
        write(nsfd, message, strlen(message));

        memset(buffer, 0, BUFFER_SIZE);  // Clear the buffer for the next message
    }

    if (read_size == 0) {
        printf("Client disconnected.\n");
    } else if (read_size < 0) {
        perror("Read failed");
    }

    // Close the socket and exit the thread
    close(nsfd);
    return NULL;
}

int main() {
    struct sockaddr_in server, client;
    int sd, nsd, clientLen;

    // Create socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;  // Listen on any interface
    server.sin_port = htons(PORT);

    // Bind the socket to the specified address and port
    if (bind(sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sd, 5) < 0) {
        perror("Listen failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (true) {
        clientLen = sizeof(client);
        nsd = accept(sd, (struct sockaddr *)&client, (socklen_t *)&clientLen);
        if (nsd < 0) {
            perror("Accept failed");
            continue; // Skip this iteration if accept fails
        }

        printf("Connected to client from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        // Allocate memory for the socket descriptor to pass to thread
        int *new_sock = malloc(sizeof(int));
        if (new_sock == NULL) {
            perror("Could not allocate memory");
            close(nsd);
            continue;
        }
        *new_sock = nsd;

        // Create a new thread for the connection
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void *)new_sock) != 0) {
            perror("Could not create thread");
            free(new_sock); // Free memory if thread creation fails
            close(nsd);
        }
        pthread_detach(thread_id); // Detach the thread to handle its cleanup automatically
    }

    // Close the server socket (unreachable in this example)
    close(sd);
    return 0;
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058$ ./a.out 
Server is listening on port 5555
Connected to client from 127.0.0.1:40786
Received from client: Hello from client
Client disconnected.
*/