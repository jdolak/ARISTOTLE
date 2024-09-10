#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define PORT 2323

int run_server() {


    int sockfd;
	// Creates a TCP socket id from IPV4 family
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		printf("Error in connection.\n");
		return(1);
	}

    struct sockaddr_in serverAddr;
	// 
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// Bind socket to port on localhost
	int ret;
	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if (ret < 0) {
		printf("Error in binding.\n");
		printf("%s\n", strerror(errno));
		exit(1);
	}

	// Listen
	if (listen(sockfd, 20) == 0) {
		printf("Listening on port %d\n", PORT);
	}

	int cnt = 0;
    int clientSocket;
    struct sockaddr_in cliAddr;
    pid_t pid;
    socklen_t addr_size;
	while (1) {

		// Accept clients and store their information in cliAddr
		clientSocket = accept(sockfd, (struct sockaddr*)&cliAddr, &addr_size);

		// Error handling
		if (clientSocket < 0) {
			printf("%s\n", strerror(errno));
            exit(1);
		}

		// Displaying information of connected client
		printf("Connection accepted from %s:%d\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));

		// Print number of clients
		// connected till now
		printf("Clients connected: %d\n\n", ++cnt);

		pid = fork();
		if (pid == 0) {// we are the child
            
			// Closing the server socket id (in the child)

			close(sockfd);
            sockfd = -1;
            FILE* clifp = fdopen(clientSocket, "r+");
            fprintf(clifp, "This is a test write to client %d\n", getpid());

			// TODO: give clifp to Jachob!

		    fclose(clifp);
            exit(0);

        } // if not the child, we are the parent, so don't handle any connections
	}

	// Close the client socket id
	close(clientSocket);
	return 0;
}

