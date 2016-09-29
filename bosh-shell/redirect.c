#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "redirect.h"

// S_IRUSR = user can read
// S_IWUSR = user can write (overwrite and delete)
// S_IRGRP = group can read
// S_IROTH = other users can read
#define FILE_RIGHTS (S_IRUSR | S_IWUSR) | S_IRGRP | S_IROTH

int redirect_stdincmd(char *infilename)
{
    /* Create a handle (file descriptor) to a file*/
    int fid = open(infilename, O_RDONLY); //Handle (file descriptor) to a file
    close(STDIN_FILENO); //close the standard input
    int new_fid = dup(fid); //Assign "fid" as new standard input
    close(fid); // The file descriptor is duplicated close the opened file
    return new_fid; //Return -1 if error
}

int redirect_stdoutcmd(char *outfilename)
{
    int fid = open(outfilename, O_CREAT|O_WRONLY|O_TRUNC, FILE_RIGHTS);
    close(STDOUT_FILENO);
    int new_fid = dup(fid);
    close(fid);
    return new_fid;
}
