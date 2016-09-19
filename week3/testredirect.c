#include <stdio.h>

#include "redirect.h"

int main(int argc, char *argv[]) {
    printf("Redirecting test.txt into cat:\n");
    redirect_stdincmd("/bin/cat", argv, "test.txt");

    printf("Redirecting standard output into output.txt:\n");
    redirect_stdoutcmd("/bin/ls", argv, "output.txt");
}
