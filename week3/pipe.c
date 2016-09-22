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

void runcmd(char *filename, char *argv[], int std_io_close, int fd_close, int fd_dup);

/* create a pipe between two new processes, executing the programs
   specified by filename1 and filename2 with the arguments in argv1
   and argv2 and wait for termination */
int pipecmd(char *filename1, char *argv1[], char *filename2, char *argv2[])
{
    int fd[2];

    // Create the pipe
    if (pipe(fd) < 0)
        exit(1);

    // Fork the process
    pid_t p_sendr = fork();
    if (p_sendr == 0) {
        // Sending child, close STDOUT and fd[0] (reading end),
        // then dup fd[1] (write end) in STDOUT's place and
        // then run the command
        runcmd(filename1, argv1, STDOUT_FILENO, fd[0], fd[1]);
        exit(0);
    } else {
        pid_t p_recvr = fork();
        if (p_recvr == 0) {
            // Receiving child, close STDIN and fd[1] (writing end),
            // then dup fd[0] (read end) in STDIN's place and
            // then run the command
            runcmd(filename2, argv2, STDIN_FILENO, fd[1], fd[0]);
            exit(0);
        } else {
            // Parent code
            // wait for the children to finish
            waitpid(p_sendr, NULL, 0);
            waitpid(p_recvr, NULL, 0);
            return 0;
        }
    }
}

void runcmd(char *filename, char *argv[], int std_io_close, int fd_close, int fd_dup) {
    close(fd_close);
    close(std_io_close);
    dup(fd_dup);
    execvp(filename, argv);
}
