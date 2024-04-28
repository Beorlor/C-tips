#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define BUFFER_SIZE 4096
char base_directory[1024] = "./";  // Default directory if none specified

void *handle_client(void *client_socket) {
    int client_fd = *(int *)client_socket;
    free(client_socket);  // Free the dynamically allocated socket pointer

    char buffer[BUFFER_SIZE] = {0};
    int bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);  // Read the request
    if (bytes_read <= 0) {
        close(client_fd);
        return NULL;  // Close connection if read error or no data
    }

    const char *method, *path;
    char *header_line = buffer;
    method = strtok_r(header_line, " ", &header_line);  // Extract method
    path = strtok_r(NULL, " ", &header_line);  // Extract path

    // Handle GET requests
    if (strcmp(method, "GET") == 0) {
        if (strcmp(path, "/") == 0) {
            const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 13\r\n\r\nHello, World!";
            write(client_fd, response, strlen(response));
        } else if (strncmp(path, "/echo/", 6) == 0) {
            path += 6;  // Skip the "/echo/" part
            char response[1024];
            int len = sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %ld\r\n\r\n%s", strlen(path), path);
            write(client_fd, response, len);
        } else if (strncmp(path, "/files/", 7) == 0) {
            char filepath[2048];
            snprintf(filepath, sizeof(filepath), "%s%s", base_directory, path + 7);
            int file_fd = open(filepath, O_RDONLY);
            if (file_fd < 0) {
                char *response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 18\r\n\r\nResource not found.";
                write(client_fd, response, strlen(response));
            } else {
                struct stat stat_buf;
                fstat(file_fd, &stat_buf);
                char response_header[1024];
                int header_len = sprintf(response_header, "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nContent-Length: %ld\r\n\r\n", stat_buf.st_size);
                write(client_fd, response_header, header_len);
                char file_buffer[BUFFER_SIZE];
                int read_bytes;
                while ((read_bytes = read(file_fd, file_buffer, BUFFER_SIZE)) > 0) {
                    write(client_fd, file_buffer, read_bytes);
                }
                close(file_fd);
            }
        } else if (strcmp(path, "/user-agent") == 0) {
            char *user_agent = NULL;
            char *line;
            while ((line = strtok_r(NULL, "\r\n", &header_line))) {
                if (strncmp(line, "User-Agent: ", 12) == 0) {
                    user_agent = line + 12;
                    break;
                }
            }
            if (user_agent) {
                char response[1024];
                int len = sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %ld\r\n\r\n%s", strlen(user_agent), user_agent);
                write(client_fd, response, len);
            } else {
                char *response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 21\r\n\r\nUser-Agent not found.";
                write(client_fd, response, strlen(response));
            }
        } else {
            char *response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 18\r\n\r\nResource not found.";
            write(client_fd, response, strlen(response));
        }
    }
    // Handle POST requests
    else if (strcmp(method, "POST") == 0 && strncmp(path, "/files/", 7) == 0) {
        path += 7;  // Skip the "/files/" part
        char filepath[2048];
        snprintf(filepath, sizeof(filepath), "%s%s", base_directory, path);
        int file_fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (file_fd < 0) {
            char *response = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\nContent-Length: 21\r\n\r\nInternal Server Error.";
            write(client_fd, response, strlen(response));
        } else {
            char *body = strstr(header_line, "\r\n\r\n") + 4;  // Find start of the body
            int body_length = bytes_read - (body - buffer);  // Calculate body length
            if (write(file_fd, body, body_length) != body_length) {
                // Handle incomplete writes
            }
            close(file_fd);
            char *response = "HTTP/1.1 201 Created\r\nContent-Length: 0\r\n\r\n";
            write(client_fd, response, strlen(response));
        }
    } else {
        char *response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 13\r\n\r\nBad Request.";
        write(client_fd, response, strlen(response));
    }

    close(client_fd);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc >= 3 && strcmp(argv[1], "--directory") == 0) {
        strncpy(base_directory, argv[2], sizeof(base_directory) - 1);
        base_directory[sizeof(base_directory) - 1] = '\0';
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
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
    printf("Server is listening for connections...\n");

    while (1) {
        int *newsockfd = malloc(sizeof(int));
        struct sockaddr_in cli_addr;
        socklen_t clilen = sizeof(cli_addr);
        *newsockfd = accept(server_fd, (struct sockaddr *)&cli_addr, &clilen);
        if (*newsockfd < 0) {
            perror("Accept failed");
            free(newsockfd);
            continue;
        }

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, newsockfd);
        pthread_detach(thread_id);
    }

    close(server_fd);
    return 0;
}

