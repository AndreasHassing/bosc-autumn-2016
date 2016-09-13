#include <stdio.h>

#include "forback.h"

int main(int argc, char* argv[]) {
    printf("Completing foregroundcmd: /bin/ls\n");
    foregroundcmd("/bin/ls", argv);

    printf("\nCompleting backgroundcmd: /bin/ls\n");
    backgroundcmd("/bin/ls", argv);
}
