#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

int main() {
    int server_fd, client_fd, client_addr_len;
    struct sockaddr_in client_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    int reuse = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    struct sockaddr_in serv_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(4221),
        .sin_addr = { htonl(INADDR_ANY) }
    };

    if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    listen(server_fd, 10);
    printf("Waiting for a client to connect...\n");

    client_addr_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0) {
        perror("Accept failed");
        return 1;
    }

    char buffer[4096];
    int bytes_read = read(client_fd, buffer, sizeof(buffer));
    buffer[bytes_read] = '\0'; // Ensure null-termination

    char *method = strtok(buffer, " ");
    char *path = strtok(NULL, " ");
    strtok(NULL, "\r\n"); // Skip past the HTTP version to the next line

    char response[1024];
    if (strcmp(path, "/") == 0) {
        sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 2\r\n\r\nOK");
    } else if (strncmp(path, "/echo/", 6) == 0) {
        char *content = path + 6;
        sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %ld\r\n\r\n%s", strlen(content), content);
    } else if (strcmp(path, "/user-agent") == 0) {
        char *line = strtok(NULL, "\r\n");
        char *user_agent = NULL;
        while (line) {
            if (strncmp(line, "User-Agent: ", 12) == 0) {
                user_agent = line + 12; // Move past the header name
                break;
            }
            line = strtok(NULL, "\r\n");
        }
        if (user_agent) {
            sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %ld\r\n\r\n%s", strlen(user_agent), user_agent);
        } else {
            strcpy(response, "HTTP/1.1 404 Not Found\r\n\r\n");
        }
    } else {
        strcpy(response, "HTTP/1.1 404 Not Found\r\n\r\n");
    }

    write(client_fd, response, strlen(response));
    close(client_fd);
    close(server_fd);

    return 0;
}

