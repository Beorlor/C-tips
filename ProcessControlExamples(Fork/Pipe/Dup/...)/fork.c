#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    pid = fork(); // Create a child process
    if (pid == -1)
    {
        perror("fork"); // If fork fails, output the error
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
        // This block is executed by the child process
        printf("This is the child process. (pid: %d)\n", getpid());
    else
        // This block is executed by the parent process
        printf("This is the parent process. (pid: %d)\n", getpid());
        int status;
        waitpid(pid, &status, 0); //protection for zombies process
    return (0);
}

/*
 * Key Concept of fork(2):
 * - fork() is a system call that creates a new process by duplicating the calling process.
 * - The new process, known as the child, is an exact copy of the parent process but has its own unique process ID.
 * - After fork(), both the parent and the child processes continue execution from the fork() call.
 * - fork() returns the child's PID to the parent process and 0 to the child process.
 * - If fork() fails, it returns -1 to the parent.
 * - In this program, the child process prints its identity ("This is the child process.") and its PID,
 *   while the parent process does the same for its own identity and PID.
 * - This demonstrates the simultaneous execution of two processes (parent and child) following a fork.
 */
