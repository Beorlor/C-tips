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
    int client_fd = *(int*)client_socket;
    free(client_socket);

    char buffer[BUFFER_SIZE];
    int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    buffer[bytes_read] = '\0'; // Ensure null-termination

    char *method = strtok(buffer, " ");
    char *path = strtok(NULL, " ");
    strtok(NULL, "\r\n"); // Skip the HTTP version

    char response[4096];
    if (strcmp(method, "GET") == 0) {
        // Handle GET request
        if (strcmp(path, "/") == 0) {
            const char* body = "Hello, World!";
            sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %ld\r\n\r\n%s", strlen(body), body);
            write(client_fd, response, strlen(response));
        } else if (strncmp(path, "/echo/", 6) == 0) {
            char *content = path + 6;
            sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %ld\r\n\r\n%s", strlen(content), content);
            write(client_fd, response, strlen(response));
        } else if (strncmp(path, "/files/", 7) == 0) {
            handle_file_get_request(path, client_fd);
        } else if (strcmp(path, "/user-agent") == 0) {
            handle_user_agent_request(client_fd, buffer);
        } else {
            send_not_found(client_fd);
        }
    } else if (strcmp(method, "POST") == 0) {
        // Handle POST request
        if (strncmp(path, "/files/", 7) == 0) {
            handle_file_post_request(path, buffer, client_fd);
        } else {
            send_not_found(client_fd);
        }
    } else {
        send_not_found(client_fd);
    }

    close(client_fd);
    return NULL;
}

void handle_file_get_request(char *path, int client_fd) {
    char filepath[2048];
    snprintf(filepath, sizeof(filepath), "%s%s", base_directory, path + 7);  // +7 to skip "/files/"
    int file_fd = open(filepath, O_RDONLY);
    if (file_fd < 0) {
        send_not_found(client_fd);
    } else {
        send_file(file_fd, client_fd);
    }
}

void handle_file_post_request(char *path, char *buffer, int client_fd) {
    char *file_content = strstr(buffer, "\r\n\r\n") + 4;  // Skip headers to get to the body
    char filepath[2048];
    snprintf(filepath, sizeof(filepath), "%s%s", base_directory, path + 7);  // +7 to skip "/files/"

    int file_fd = open(filepath, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (file_fd < 0) {
        send_not_found(client_fd);
    } else {
        write(file_fd, file_content, strlen(file_content));
        close(file_fd);
        sprintf(buffer, "HTTP/1.1 201 Created\r\nContent-Length: 0\r\n\r\n");
        write(client_fd, buffer, strlen(buffer));
    }
}

void send_file(int file_fd, int client_fd) {
    struct stat stat_buf;
    fstat(file_fd, &stat_buf);
    char header[1024];
    sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nContent-Length: %ld\r\n\r\n", stat_buf.st_size);
    write(client_fd, header, strlen(header));
    char file_buffer[BUFFER_SIZE];
    int bytes_read;
    while ((bytes_read = read(file_fd, file_buffer, BUFFER_SIZE)) > 0) {
        write(client_fd, file_buffer, bytes_read);
    }
    close(file_fd);
}

void send_not_found(int client_fd) {
    char response[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 18\r\n\r\nResource not found.";
    write(client_fd, response, strlen(response));
}

void handle_user_agent_request(int client_fd, char *buffer) {
    char *line, *user_agent = NULL;
    while ((line = strtok(NULL, "\r\n")) != NULL) {
        if (strncmp(line, "User-Agent: ", 12) == 0) {
			user_agent = line + 12;
			break;
		}
	}
	if (user_agent) {
		char response[1024];
		sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %ld\r\n\r\n%s", strlen(user_agent), user_agent);
		write(client_fd, response, strlen(response));
	} else {
		char response[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 21\r\n\r\nUser-Agent not found.";
		write(client_fd, response, strlen(response));
	}
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
