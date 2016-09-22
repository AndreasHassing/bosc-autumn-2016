#include <stdio.h>

#include "pipe.h"

int main(int argc, char *argv[]) {
    printf("Piping ls into wc -w: `ls | wc -w`\n");
    char* ls_flags[] = {"-l", "-a"};
    char* wc_flags[] = {"-w"};

    pipecmd("/bin/ls", ls_flags, "/usr/bin/wc", wc_flags);
    return 0;
}
