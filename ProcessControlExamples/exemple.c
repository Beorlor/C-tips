/*
 * This program demonstrates how to properly execute a command in a child process in C
 * and return its output to the parent process. It uses fork() to create a child process,
 * pipe for inter-process communication, and execve to execute the command in the child
 * process. The parent process reads the output of the command executed by the child
 * process through the pipe and prints it to its own standard output. This example
 * specifically executes the 'ping' command to ping 'google.com' and demonstrates
 * IPC and process control mechanisms in C.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

int main() {
    int pipefd[2];  // Array to hold the two ends of the pipe
    pid_t cpid;     // Variable to store the child process ID
    char buffer[1024];  // Buffer to store the data read from the pipe
    int status;     // Variable to store the child process's exit status

    // Creating a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Creating a child process
    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {    // Child process
        // Close the unused read end of the pipe
        close(pipefd[READ_END]);

        // Redirect STDOUT to the write end of the pipe
        dup2(pipefd[WRITE_END], STDOUT_FILENO);

        // Prepare arguments for execve
        char *args[] = {"/bin/ping", "-c", "4", "google.com", NULL};
        char *envp[] = {NULL};

        // Execute the ping command
        if (execve("/bin/ping", args, envp) == -1) {
            perror("execve");
            exit(EXIT_FAILURE);
        }

        // Close the write end of the pipe and exit
        close(pipefd[WRITE_END]);
        exit(EXIT_SUCCESS);

    } else {            // Parent process
        // Close the unused write end of the pipe
        close(pipefd[WRITE_END]);

        // Reading data sent by the child process from the pipe
        int nbytes;
        while ((nbytes = read(pipefd[READ_END], buffer, sizeof(buffer))) > 0) {
            // Write the data read from the pipe to the standard output
            write(STDOUT_FILENO, buffer, nbytes);
        }

        // Close the read end of the pipe
        close(pipefd[READ_END]);

        // Wait for the child process to finish and get its status
        waitpid(cpid, &status, 0);

        // Check how the child process exited
        if (WIFEXITED(status)) {
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child terminated by signal %d\n", WTERMSIG(status));
        }
    }

    return 0;
}
