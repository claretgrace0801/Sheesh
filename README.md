# OS Assignment 2 - Shell Part 1
## This is a shell 'Sheesh' made in C. All required functionality in part 1 has been implemented.
## File Contents
- makefile
run `make` inside the directory `2019113004_Assignment2`. This compiles and runs the shell.

- header.h
Contains all the declarations of the functions used globally throughout the shell.
- main.c
This is the main loop of the shell.
- helpers.c
This contains helper function required throughout the shell.
-  prompt.c
This contains function for displaying the shell prompt.
- parser.c
This contains functions for parsing the input commands.
- jobs.c
This contains the functions required start execution of the input commands. Background/Foreground handling is also done here.
- `cd.c`, `echo.c`, `ls.c`, `pinfo.c`, `repeat.c`
These contain definitions  of the required user-defined functions. Note: `cd.c` also contains definition of pwd command.

### Execution Method:
- An infinite while is started, which takes user input.
- This input string is passed to a function, which parses it into an array of array of strings. The outer arrays represent semicolon-separated commands and the inner array of strings represents words of each individual commands.
- This array of array of strings (the job queue) is passed to a executer function which loops over the command array and executes them individually.
- While executing, the function decides whether the command is implemented by me, in which case it called the required functions. If it is not implemented by me, `execvp` is used to execute it. 
- It also checks for `&` and decided whether to execute it as a background or foreground process. 
- All this is now repeated in the while loop.