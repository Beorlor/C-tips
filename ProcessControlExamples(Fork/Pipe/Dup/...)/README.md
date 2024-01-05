# Process Control Examples in C

This repository, `ProcessControlExamples`, contains a collection of C programs demonstrating various Unix process control and inter-process communication (IPC) mechanisms. Each example focuses on a specific system call or concept, providing a practical illustration of its usage.

## Contents

- `fork.c`: Illustrates the use of the `fork()` system call to create child processes.
- `pipe.c`: Demonstrates IPC using unnamed pipes with `pipe()` and how parent and child processes can communicate.
- `dup2.c`: Showcases file descriptor duplication and redirection using `dup2()`.
- `execve.c`: An example of executing a separate program within a process using `execve()`.
- `unlink.c`: Demonstrates file deletion and link count manipulation using `unlink()`.
- `wait_sleep.c`: Provides an example of process synchronization using `wait()` and process pausing with `sleep()`.
- `access.c`: Checks file access permissions using `access()`.

## Getting Started

To run these examples, clone the repository and compile each program with a C compiler like `gcc`. For example:

```bash
gcc -o fork fork.c
```
```bash
./fork
```

Replace `fork.c` with the name of the file you wish to compile and run.

## Prerequisites

- A Unix-like operating system (Linux, macOS, etc.)
- GCC or another C compiler installed on your system

## Acknowledgements

This repository is created for educational purposes and to provide practical examples for students and programmers learning about process control in Unix-like operating systems.

Special thanks to [Herbie Vine](https://twitter.com/herbievine) for the examples from (https://reactive.so/post/42-a-comprehensive-guide-to-pipex) pipex guide.

---

*Note: The examples provided are for educational purposes and may require modifications for production use.*
