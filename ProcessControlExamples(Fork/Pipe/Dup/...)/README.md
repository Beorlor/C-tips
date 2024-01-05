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

gcc -o fork fork.c
./fork

Replace `fork.c` with the name of the file you wish to compile and run.

## Prerequisites

- A Unix-like operating system (Linux, macOS, etc.)
- GCC or another C compiler installed on your system

## Contributing

Contributions to this repository are welcome. Please feel free to submit pull requests or open issues to discuss proposed changes or additions.

## License

This project is open source and available under the [MIT License](LICENSE).

## Acknowledgements

This repository is created for educational purposes and to provide practical examples for students and programmers learning about process control in Unix-like operating systems.

---

*Note: The examples provided are for educational purposes and may require modifications for production use.*
