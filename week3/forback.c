/*

   Opgave 1

   forback.c

 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "forback.h"

/* start the program specified by filename with the arguments in argv
   in a new process and wait for termination */
int foregroundcmd(char *filename, char *argv[])
{
    pid_t pid = fork();
    if (pid == 0) {
        // child process
        return execvp(filename, argv);
    } else {
        int status;
        wait(&status);
        return status;
    }
}

/* start the program specified by filename with the arguments in argv
   in a new process */
int backgroundcmd(char *filename, char *argv[])
{
    pid_t pid = fork();
    if (pid == 0) {
        // child process
        return execvp(filename, argv);
    }

    exit(0);
}

