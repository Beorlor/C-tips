/*
 * This C program demonstrates inter-process communication using a pipe.
 * It creates a parent and a child process using fork(). The child process
 * sends a message to the parent process through the pipe.
 *
 * Key Concepts:
 * - pipe(): Creates a pipe for inter-process communication.
 * - fork(): Splits the current process into a parent and a child process.
 * - close(): Closes a file descriptor so that it no longer refers to any file and may be reused.
 * - write(): Writes data to a file descriptor.
 * - read(): Reads data from a file descriptor.
 *
 * Workflow:
 * 1. The pipe() function is called to create a pipe with two file descriptors:
 *    fd[0] for reading and fd[1] for writing.
 * 2. The fork() function creates a new process. It returns the child's PID to the parent process
 *    and 0 to the child process.
 * 3. In the child process (where fork() returns 0), the read end of the pipe (fd[0]) is closed,
 *    and a message is written to the write end of the pipe (fd[1]). Then, the write end is closed.
 * 4. In the parent process, the write end of the pipe (fd[1]) is closed. The parent then reads
 *    the message from the read end of the pipe (fd[0]) and closes it after reading.
 * 5. The parent process prints the message received from the child process.
 *
 * The program demonstrates a one-way communication from child to parent using a pipe.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
 
int main()
{
	int fd[2];
	pid_t pid;
	char buffer[13];
 
	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
 
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
 
	if (pid == 0)
	{
		close(fd[0]); // close the read end of the pipe
		write(fd[1], "Hello parent!", 13);
		close(fd[1]); // close the write end of the pipe
		exit(EXIT_SUCCESS);
	}
	else
	{
		close(fd[1]); // close the write end of the pipe
		read(fd[0], buffer, 13);
		close(fd[0]); // close the read end of the pipe
		printf("Message from child: '%s'\n", buffer);
		wait(NULL); //wait for the child process to end
		exit(EXIT_SUCCESS);
	}
}