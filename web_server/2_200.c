#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main() {
	// Disable output buffering
	setbuf(stdout, NULL);

	// You can use print statements as follows for debugging, they'll be visible when running tests.
	printf("Logs from your program will appear here!\n");

	// Uncomment this block to pass the first stage
	//
	int server_fd, client_addr_len;
	struct sockaddr_in client_addr;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		printf("Socket creation failed: %s...\n", strerror(errno));
		return 1;
	}

	// Since the tester restarts your program quite often, setting REUSE_PORT
	// ensures that we don't run into 'Address already in use' errors
	int reuse = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) {
		printf("SO_REUSEPORT failed: %s \n", strerror(errno));
		return 1;
	}

	struct sockaddr_in serv_addr = { .sin_family = AF_INET ,
									 .sin_port = htons(4221),
									 .sin_addr = { htonl(INADDR_ANY) },
									};

	if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
		printf("Bind failed: %s \n", strerror(errno));
		return 1;
	}

	int connection_backlog = 5;
	if (listen(server_fd, connection_backlog) != 0) {
		printf("Listen failed: %s \n", strerror(errno));
		return 1;
	}

	// 200 respond
    // Accept a connection from a client
	int client_fd;
    printf("Waiting for a client to connect...\n");
    client_addr_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
    if (client_fd < 0) {
        printf("Accept failed: %s\n", strerror(errno));
        return 1;
    }
    printf("Client connected\n");

    // Read data from the client and ignore it (for now)
    char buffer[1024];
    int bytes_read = read(client_fd, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        printf("Read failed: %s\n", strerror(errno));
        close(client_fd);
        close(server_fd);
        return 1;
    }

    // Prepare and send the HTTP response
    const char *http_response = "HTTP/1.1 200 OK\r\n\r\n";
    int bytes_written = write(client_fd, http_response, strlen(http_response));
    if (bytes_written < 0) {
        printf("Write failed: %s\n", strerror(errno));
        close(client_fd);
        close(server_fd);
        return 1;
    }

    // Close the client socket
    close(client_fd);
    // Close the server socket
    close(server_fd);

	return 0;
}
