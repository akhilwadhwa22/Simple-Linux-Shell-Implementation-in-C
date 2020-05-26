# Simple-Linux-Shell-Implementation-in-C

## Description
A simple implementation of Linux Shell using C. 

Set of Implemented built-in functionalities:
```
"cd": change directory inside the implemented shell. (does not change the directory of the parent shell)
"history": Displays the last 25 unique user input commands.
"clear": clears the screen
"help": opens a helper
"exit": exit the shell to the parent shell.

```
The current implementation uses fork to implement all the other functionalities of a shell in a child process.
## Prerequisites

1. Linux/unix system
2. C compiler

## Execution commands

```
gcc -o myshell myshell.c

./myshell

```
