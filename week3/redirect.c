/*

   Opgave 2

   redirect.c

 */

#include <sys/types.h> // pid_t
#include <fcntl.h>     // open, file permissions

#include <unistd.h>    // execvp
#include <sys/wait.h>  // wait

#include "redirect.h"

// S_IRUSR = user can read
// S_IWUSR = user can write (overwrite and delete)
// S_IRGRP = group can read
// S_IROTH = other users can read
#define FILE_RIGHTS (S_IRUSR | S_IWUSR) | S_IRGRP | S_IROTH

/* start the program specified by filename with the arguments in argv
   in a new process that has its stdin redirected to infilename and
   wait for termination */
int redirect_stdincmd(char *filename, char *argv[], char *infilename)
{
    pid_t pid = fork();
    if (pid == 0) {
        int fid = open(infilename, O_RDONLY);

        // replace stdin
        close(STDIN_FILENO);
        dup(fid);

        close(fid);

        return execvp(filename, argv);
    } else {
        int status;
        wait(&status);
        return status;
    }
}

/* start the program specified by filename with the arguments in argv
   in a new process that has its stdout redirected to outfilename and
   wait for termination */
int redirect_stdoutcmd(char *filename, char *argv[], char *outfilename)
{
    pid_t pid = fork();
    if (pid == 0) {
        int fid = open(outfilename, O_WRONLY | O_CREAT, FILE_RIGHTS);

        // replace stdout
        close(STDOUT_FILENO);
        dup(fid);

        close(fid);

        return execvp(filename, argv);
    } else {
        int status;
        wait(&status);
        return status;
    }
}
