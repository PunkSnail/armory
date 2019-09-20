#include <stdio.h>

#include "formatted_text.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("missing target file...\n");
    }
    formatted_text(argv[1]);

    return 0;
}
