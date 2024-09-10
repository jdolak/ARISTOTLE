#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 23
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

void *handle_client(void *client_socket);

int make_server() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    pthread_t tid[MAX_CLIENTS];
    int client_count = 0;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket error");
        return 1;
    }

    // Server address setup
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind and listen
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind error");
        return 1;
    }
    listen(server_socket, 10);

    printf("BBS Server is running, waiting for connections...\n");

    while (client_count < MAX_CLIENTS) {
        addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);

        if (client_socket < 0) {
            perror("Accept error");
            return 1;
        }

        // Handle new client connection
        if (pthread_create(&tid[client_count], NULL, handle_client, &client_socket) != 0) {
            printf("Failed to create thread\n");
        }
        client_count++;
    }

    close(server_socket);
    return 0;
}

void *handle_client(void *client_socket) {
    int sock = *(int*)client_socket;
    char buffer[BUFFER_SIZE];
    int bytes_read;

    // Send welcome message
    char *welcome_message = "Welcome to the BBS server!\r\n";
    send(sock, welcome_message, strlen(welcome_message), 0);

    // Simple echo loop
    while ((bytes_read = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[bytes_read] = '\0';
        printf("Client: %s", buffer);  // Log client messages
        send(sock, buffer, strlen(buffer), 0);  // Echo back
    }

    close(sock);
    return NULL;
}