#include <stdio.h>

#include "pipe.h"

int main(int argc, char *argv[]) {
    printf("Piping ls into wc -w: `ls | wc -w`\n");
    char* ls_flags[] = {argv[0], "-l", "-a", NULL};
    char* wc_flags[] = {argv[0], "-w", NULL};

    pipecmd("ls", ls_flags, "wc", wc_flags);
    return 0;
}
