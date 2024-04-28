#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#define BUFFER_SIZE 4096

void *handle_client(void *client_socket) {
    int client_fd = *(int*)client_socket;
    free(client_socket);

    char buffer[BUFFER_SIZE];
    int bytes_read = read(client_fd, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        perror("Error reading from socket");
        close(client_fd);
        return NULL;
    }
    buffer[bytes_read] = '\0'; // Ensure null-termination

    char *method = strtok(buffer, " ");
    char *path = strtok(NULL, " ");
    strtok(NULL, "\r\n"); // Skip the HTTP version

    char response[2048];
    if (strcmp(path, "/") == 0) {
        sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 13\r\n\r\nHello, World!");
        write(client_fd, response, strlen(response));
    } else if (strncmp(path, "/echo/", 6) == 0) {
        char *content = path + 6;
        sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %ld\r\n\r\n%s", strlen(content), content);
        write(client_fd, response, strlen(response));
    } else if (strcmp(path, "/user-agent") == 0) {
        char *line;
        char *user_agent = NULL;
        while ((line = strtok(NULL, "\r\n")) != NULL) {
            if (strncmp(line, "User-Agent: ", 12) == 0) {
                user_agent = line + 12; // Move past the header name
                break;
            }
        }
        if (user_agent) {
            sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %ld\r\n\r\n%s", strlen(user_agent), user_agent);
        } else {
            strcpy(response, "HTTP/1.1 404 Not Found\r\n\r\n");
        }
        write(client_fd, response, strlen(response));
    } else {
        strcpy(response, "HTTP/1.1 404 Not Found\r\n\r\n");
        write(client_fd, response, strlen(response));
    }

    close(client_fd);
    return NULL;
}

int main() {
    int server_fd, *client_fd, client_addr_len;
    struct sockaddr_in client_addr, serv_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    int reuse = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(4221);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    listen(server_fd, 10);
    printf("Server is listening for connections...\n");

    while (1) {
        client_fd = malloc(sizeof(int));
        client_addr_len = sizeof(client_addr);
        *client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (*client_fd < 0) {
            perror("Accept failed");
            free(client_fd);
            continue;
        }

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, client_fd) != 0) {
            perror("Failed to create thread");
            close(*client_fd);
            free(client_fd);
        }
        pthread_detach(thread_id); // Allow threads to clean up after themselves
    }

    close(server_fd);
    return 0;
}

