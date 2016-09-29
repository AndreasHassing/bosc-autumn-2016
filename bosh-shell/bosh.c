/*

   bosh.c : BOSC shell

 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parser.h"

#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "redirect.h"

/* --- symbolic constants --- */
#define HOSTNAMEMAX 100
static int SAVED_STDIN;
static int SAVED_STDOUT;

static pid_t pid_to_interrupt = 0;

void propagate_signal(int sig) {
    // kill seems aggressive, but all it does is
    // to send a signal (2nd argument) to another
    // process
    kill(pid_to_interrupt, sig);
}

/* --- use the /proc filesystem to obtain the hostname --- */
char *gethostname2(char *hostname)
{
    FILE *hostnamefile;
    hostnamefile = fopen("/proc/sys/kernel/hostname", "r");
    fgets(hostname, HOSTNAMEMAX, hostnamefile);
    fclose(hostnamefile);

    // remove trailing newline from hostname
    sscanf(hostname, "%[^\n]", hostname);

    return hostname;
}

/*Restore std IO to terminal*/
int restore_std_IO() {
    dup2(SAVED_STDIN, STDIN_FILENO);
    close(SAVED_STDIN);
    dup2(SAVED_STDOUT, STDOUT_FILENO);
    close(SAVED_STDOUT);
    return 0;
}

/*Execute command with terminal IO*/
void execute(Cmd *cmdlist) {
    char **cmd = cmdlist->cmd;
    cmdlist = cmdlist->next;

    pid_t pid = -1;
    int fd[2];

    // any more commands remaining?
    if (cmdlist) {
        // if so, setup the pipe
        if (pipe(fd) < 0) {
            fprintf(stderr, "Failed to setup pipe");
            exit(1);
        }
        pid = fork();
    }

    if (pid == 0) {
        // pipe sender
        close(STDOUT_FILENO);
        close(fd[0]);
        dup(fd[1]);

        execute(cmdlist);
    } else {
        if (cmdlist) {
            // pipe receiver
            close(STDIN_FILENO);
            close(fd[1]);
            dup(fd[0]);
        }

        execvp(*cmd, cmd);
        switch (errno) {
            case ENOENT:
                fprintf(stderr, "command not found\n");
                exit(ENOENT);
                break;
            case EACCES:
                fprintf(stderr, "permission to run %s denied\n", *cmd);
                exit(EACCES);
                break;
        }

        exit(0);
    }
}

/* --- execute a shell command --- */
int executeshellcmd (Shellcmd *shellcmd)
{
    Cmd *cmdlist = shellcmd->the_cmds;

    char **cmd = cmdlist->cmd;

    // if builtin command `exit` is run, kill the shell
    if (!strcmp(*cmd, "exit"))
        return 1;

    pid_t pid = fork();
    if (pid == 0) {
        if (shellcmd->rd_stdin) {
            /*Redirect stdin*/
            if(redirect_stdincmd(shellcmd->rd_stdin) == -1) {
                fprintf(stderr, "Unable to redirect stdin to %s\n", shellcmd->rd_stdin);
                exit(0);
            }
        }
        if (shellcmd->rd_stdout) {
            /*Redirect stdout*/
            if(redirect_stdoutcmd(shellcmd->rd_stdout) == -1) {
                fprintf(stderr, "Unable to redirect stdout to %s\n", shellcmd->rd_stdout);
                exit(0);
            }
        }
        if (shellcmd->background) {
            // divorce from parent process by forking child again,
            // and killing the middle-child
            int child;
            child = fork();
            if (child < 0)
                exit(EXIT_FAILURE);
            if (child > 0)
                exit(EXIT_SUCCESS);
            // set new child as process group leader, unless something bad happens
            if (setsid() < 0)
                exit(EXIT_FAILURE);
            printf("Background process ID: %d\n", getpid());
        }

        execute(cmdlist);
    } else {
        // interrupt ctrl+c, and propagate it to command process
        pid_to_interrupt = pid;
        signal(SIGINT, propagate_signal);

        waitpid(pid, NULL, 0);
        // revert the interrupt signal so bosh can exit
        signal(SIGINT, SIG_DFL);
    }

    return 0;
}

/* --- main loop of the simple shell --- */
int main(int argc, char* argv[]) {

    /* initialize the shell */
    char *cmdline;
    char hostname[HOSTNAMEMAX];
    int terminate = 0;
    SAVED_STDIN = dup(STDIN_FILENO); //Save current stdin
    SAVED_STDOUT = dup(STDOUT_FILENO);
    Shellcmd shellcmd;

    if (gethostname2(hostname)) {

        /* parse commands until exit or ctrl-c */
        while (!terminate) {
            printf("%s", hostname);
            if ((cmdline = readline(":# "))) {
                if(*cmdline) {
                    add_history(cmdline);
                    if (parsecommand(cmdline, &shellcmd)) {
                        terminate = executeshellcmd(&shellcmd);
                        restore_std_IO();
                    }
                }
                free(cmdline);
            } else terminate = 1;
        }
        printf("Exiting bosh.\n");
    }

    return EXIT_SUCCESS;
}
