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

    char buffer[1024];
    read(client_fd, buffer, sizeof(buffer));

    char *method = strtok(buffer, " ");
    char *path = strtok(NULL, " ");

    printf("Requested path: %s\n", path);

    const char *response_header;
    char response_body[1024];
    if (strncmp(path, "/echo/", 6) == 0) {
        char *content = path + 6;
        sprintf(response_body, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %ld\r\n\r\n%s", strlen(content), content);
    } else if (strcmp(path, "/") == 0) {
        response_header = "HTTP/1.1 200 OK\r\n\r\n";
        sprintf(response_body, "%s", response_header);
    } else {
        response_header = "HTTP/1.1 404 Not Found\r\n\r\n";
        sprintf(response_body, "%s", response_header);
    }

    write(client_fd, response_body, strlen(response_body));
    close(client_fd);
    close(server_fd);

    return 0;
}

