/*

   Opgave 3

   pipe.c

 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "pipe.h"

/* create a pipe between two new processes, executing the programs
   specified by filename1 and filename2 with the arguments in argv1
   and argv2 and wait for termination */
int pipecmd(char *filename1, char *argv1[], char *filename2, char *argv2[])
{
    int nchar, fd[2];
    char buffer[128];

    // Create the pipe
    if (pipe(fd) < 0)
        exit(1);

    pid_t child1 = fork();
    if (child1 == 0) {
        // Child 1 code
        close(fd[0]); // close the reading end
        // TODO: create the message to be sent via the pipe
        write(fd[1], message, sizeof(message));
        exit(0);
    } else {
        pid_t child2 = fork();
        if (child2 == 0) {
            // Child 2 code
            close(fd[1]); // close the writing end
            nchar = read(fd[0], buffer, sizeof(buffer));
            // TODO: read the message from the pipe buffer
            exit(0);
        } else {
            // Parent code
            int status;
            wait(&status);
            return status;
        }
    }
}
