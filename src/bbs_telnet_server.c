#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#include "base.h"

#define PORT 2323
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int server_socket;
int client_sockets[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;


void *handle_client(void *client_socket);
void handle_sigint(int sig);
void add_client(int client_socket);
void remove_client(int client_socket);
void broadcast_message(const char *message);

int make_server() {
    int client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    pthread_t tid[MAX_CLIENTS];
    int client_count = 0;
    int opt = 1;

    // Register signal handler for SIGINT
    signal(SIGINT, handle_sigint);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket error");
        return 1;
    }

    // Set socket options
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Setsockopt error");
        return 1;
    }

    // Server address setup
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind error");
        return 1;
    }

    // Listen for connections
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listen error");
        return 1;
    }

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
        if (client_socket < 0) {
            perror("Accept error");
            return 1;
        }

        if (pthread_create(&tid[client_count++], NULL, handle_client, &client_socket) != 0) {
            perror("Pthread_create error");
            return 1;
        }

        if (client_count >= MAX_CLIENTS) {
            client_count = 0;
            while (client_count < MAX_CLIENTS) {
                pthread_join(tid[client_count++], NULL);
            }
            client_count = 0;
        }
    }

    return 0;
}

void handle_sigint(int sig) {
    printf("Caught signal %d, closing server socket\n", sig);
    close(server_socket);
    exit(0);
}

void *handle_client(void *client_socket) {
    int sock = *(int*)client_socket;
    //char buffer[BUFFER_SIZE];
    //int bytes_read;

    add_client(sock);

    // Send welcome message
    char *welcome_message = "Welcome to the BBS server!\r\n";
    send(sock, welcome_message, strlen(welcome_message), 0);

    // Notify all clients about the new connection
    char doorbell_message[] = "A new user has connected to the server.\n";
    broadcast_message(doorbell_message);

    // Simple echo loop
   // while ((bytes_read = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
   //     buffer[bytes_read] = '\0';
   //     printf("Client: %s", buffer);  // Log client messages
   //     send(sock, buffer, strlen(buffer), 0);  // Echo back
   // }

    start_screen(sock);
    
    remove_client(sock);
    close(sock);
    return NULL;
}

void add_client(int client_socket) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (client_sockets[i] == 0) {
            client_sockets[i] = client_socket;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int client_socket) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (client_sockets[i] == client_socket) {
            client_sockets[i] = 0;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void broadcast_message(const char *message) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (client_sockets[i] != 0) {
            send(client_sockets[i], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}