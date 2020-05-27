# Simple-Linux-Shell-Implementation-in-C
![](https://github.com/akhilwadhwa22/Simple-Linux-Shell-Implementation-in-C/blob/master/startup_page.png)
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
The current implementation uses fork to implement all the other standard functionalities of a shell in a child process.

## Prerequisites

```
Linux/unix system
C compiler
```

## Execution commands

```
gcc -o myshell myshell.c

./myshell
```
## Improvements
- working on implementing a more optimized buffer for storing history. Some of the potential implementation:
```
LRU (Least Recently Used) 
Circular Linked-List
```
-Working towards implemeting more builtin functions.
-Working on adding functionality for pipes
