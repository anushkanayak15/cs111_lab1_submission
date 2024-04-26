## UID: 605977416

## Pipe Up

This project aims to simulate the functionality of the pipe ('|') operator in shells. Users will pass executable names as command-line arguments, and the program will execute each one in a new process, similar to a shell. 


## Building

To build the program, simply compile the code using :
```shell
make
```
This creates a binary to run the program. 

## Running

To run the program, execute it from the command line specifying the commands to be executed:

```shell
./pipe cmd1  cmd2  cmd3
```
cmd1, cmd2, and cmd3 represent the commands to be executed in sequence, with the output of each command piped to the next.

For example: 

```shell
./pipe ls sort
```
Expected Output: 
The above  will take the output of the ls command and pipe it to the sort command. The expected output could be something like this: 

Makefile
pipe
pipe.c
pipe.o
README.md
test_lab1.py

The above output matches the output that the actual pipe command ('|') would have given: 
```shell
ls | sort
```
Similarly, more arguments can also be passed:

```shell
./pipe ls cat wc sort
```
Expected Output: 
7       7      60

```shell
./pipe ls cat sort uniq wc id pwd wc wc wc 
```
Expected Output: 
1       3      24

## Cleaning up

To remove all binary files generated during the build process, run:
```shell
make clean
```
This command deletes all binary and object files, ensuring a clean workspace. Then run 'make' again. 
