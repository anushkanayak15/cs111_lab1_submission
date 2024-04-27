#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    // first need to check if correct number of arguments have been provided

    if (argc < 2)
    {
        // fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        // there should be at least two arguments. one of the arguments is pipe itself
        return EINVAL; // invalid no of args
    }

    int pipefd[2];
    // need to create a pipe
    pipe(pipefd); // creates a unidirectional communication channel between the parent and child processes

    // to ensure that the pipe was created successfully
    if (pipe(pipefd) == -1)
    {
        // fprintf(stderr, "Pipe creation failed\n");
        exit(errno); // if pipe creation failed then give out the error code
    }

    // create a loop for multiple command execution
    int curr_i = 1; // current index starts at 1 to skip over program name itself (pipe)

    while (curr_i < argc) // the loop goes on till the last argument is reached
    {
        int ret = fork(); // fork create a child process
        // fork system creates a new process by duplicating an exsisting process creates two identical process-> parent and child
        // parent and the child processes exist concurrently.

        // check if the fork was created successfully

        if (ret < 0) // fork created not successfully
        {
            // fprintf(stderr, "Fork failed\n");
            exit(errno); // return error if fork creation fails
        }

        if (ret == 0) // fork created successfully and is in the child process
        {             // Child process
            // If not the last command, redirect stdout to the write end of the pipe
            if (curr_i != argc - 1)
            {
                // redirection sends the std output of the current command to the write end of the pipe if it is not the last command in the list
                close(pipefd[0]); // Close unused read end of the pipe
                if (dup2(pipefd[1], STDOUT_FILENO) == -1)
                // duplicates file descriptor for the write end of the pipe (pipefd[1]) onto the standard output file descriptor (STDOUT_FILENO)
                {
                    // fprintf(stderr, "Duplication of file descriptor failed\n");
                    exit(errno);
                }
                close(pipefd[1]); // Close write end of the pipe
            }

            execlp(argv[curr_i], argv[curr_i], NULL); // executes the current command
            // fprintf(stderr, "Execution of command failed\n");
            exit(errno); //  if execlp fails then child process exits with an error code
        }

        else
        { // Parent process
            int status;
            waitpid(ret, &status, 0); // wait for the child process to finish

            if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            {
                return WEXITSTATUS(status); // Return the exit status if the child process exited with an error
            }

            if (curr_i != argc - 1) // if current command is not in the list. set up the redirection
            {
                close(pipefd[1]);                        // Close unused write end of the pipe
                if (dup2(pipefd[0], STDIN_FILENO) == -1) // duplicate file descriptor for the read end of the pipe (pipefd[0]) onto the standard input file descriptor (STDIN_FILENO)
                {
                    // fprintf(stderr, "Duplication of file descriptor failed\n");
                    exit(errno);
                }
                close(pipefd[0]); // Close read end of the pipe

                if (pipe(pipefd) == -1) // create a new pipe successfully
                {
                    // fprintf(stderr, "Pipe creation failed\n");
                    exit(errno);
                }
            }
        }

        curr_i++; // Move to the next command index
    }

    return 0;
}
