# Unix Shell

A Unix-like command-line shell implemented in C that supports command execution, process management, built-in commands, and command history.

This project demonstrates fundamental operating system concepts, including process creation, system calls, signal handling, and foreground/background execution.

## Features

### Command Execution
- Execute external Unix commands with arguments.
- Create child processes using `fork()` and execute programs using the `exec` family of functions.
- Support foreground execution using `waitpid()`.
- Support background execution using the `&` operator.
- Clean up terminated background processes to prevent zombie processes.

### Built-in Commands

| Command | Description |
|---------|-------------|
| `cd` | Change the current working directory |
| `pwd` | Display the current working directory |
| `exit` | Exit the shell |
| `help` | Display information about supported commands |
| `history` | Display the 10 most recent commands |
| `!!` | Re-execute the previous command |
| `!n` | Re-execute command number `n` from history |

### Signal Handling
- Handle `SIGINT` (Ctrl+C) without terminating the shell.
- Display help information when an interrupt signal is received.

### Command History
- Store the 10 most recently executed commands.
- Display command history in reverse chronological order.
- Support re-execution of previously entered commands.

## Technologies Used

- **C** — Core implementation
- **POSIX System Calls** — Process creation, execution, and management
- **CMake** — Build system
- **Linux/Unix** — Target environment

## Installation

### Prerequisites

- Linux or a Unix-compatible environment (such as WSL)
- C compiler (GCC or Clang)
- CMake

### Build

Clone the repository:

```bash
git clone https://github.com/jaskiratchouhan/unix-shell.git
cd unix-shell
```

Configure and build the project:

```bash
cmake -S . -B build
cmake --build build
```

Run the shell:

```bash
./build/shell
```

## Usage

Once the shell is running, users can enter commands at the prompt.

**Execute an external command:**

```bash
$ ls
```

**Change directories:**

```bash
$ cd Documents
$ pwd
```

**Run a background process:**

```bash
$ sleep 10 &
```

**View command history:**

```bash
$ history
```

**Re-execute the previous command:**

```bash
$ !!
```

**Exit the shell:**

```bash
$ exit
```

## Project Structure

```text
unix-shell/
├── src/
│   └── task0.c
├── include/
│   └── msgs.h
├── CMakeLists.txt
└── README.md
```

## Limitations

This is a simplified Unix shell intended to demonstrate operating system fundamentals.

It does not currently support:
- Command pipelines (`|`)
- Input/output redirection (`<`, `>`)
- Advanced shell scripting

## Project Background

Originally developed as part of CMPT 201 coursework at Simon Fraser University, created by Mohamed Hefeeda, modified by Brian Fraser, Keval Vora, Tianzheng Wang, and Steve Ko. The project focuses on implementing fundamental shell functionality using C and POSIX system calls.

