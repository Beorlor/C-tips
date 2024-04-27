#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main() {
    setbuf(stdout, NULL);
    printf("Logs from your program will appear here!\n");

    int server_fd, client_fd, client_addr_len;
    struct sockaddr_in client_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        printf("Socket creation failed: %s...\n", strerror(errno));
        return 1;
    }

    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) {
        printf("SO_REUSEPORT failed: %s\n", strerror(errno));
        return 1;
    }

    struct sockaddr_in serv_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(4221),
        .sin_addr = { htonl(INADDR_ANY) }
    };

    if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
        printf("Bind failed: %s\n", strerror(errno));
        return 1;
    }

    if (listen(server_fd, 5) != 0) {
        printf("Listen failed: %s\n", strerror(errno));
        return 1;
    }

    printf("Waiting for a client to connect...\n");
    client_addr_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
    if (client_fd < 0) {
        printf("Accept failed: %s\n", strerror(errno));
        return 1;
    }
    printf("Client connected\n");

    char buffer[1024];
    int bytes_read = read(client_fd, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        printf("Read failed: %s\n", strerror(errno));
        close(client_fd);
        close(server_fd);
        return 1;
    }
    buffer[bytes_read] = '\0'; // Null-terminate whatever was read

    // Parse the request
    char *path = strtok(buffer, " "); // Extracting the first token (GET)
    path = strtok(NULL, " "); // Extracting the path, second token

    // Prepare the response based on the path
    const char *response;
    if (strcmp(path, "/") == 0) {
        response = "HTTP/1.1 200 OK\r\n\r\n";
    } else {
        response = "HTTP/1.1 404 Not Found\r\n\r\n";
    }

    // Send the response
    write(client_fd, response, strlen(response));

    // Close the client socket
    close(client_fd);
    // Close the server socket
    close(server_fd);

    return 0;
}

