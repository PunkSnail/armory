#include <stdio.h>
#include <getopt.h> /* getopt_long */

#include "formatted_text.h"

void show_help(void)
{
    printf("Options:\n"
           "-f <file>   Format the specified file (add spaces)\n"
           "-e <file>   Eat the trailing space in the specified file\n"
           /* "-o <file>   Place the output into <file>\n" */
           "--help      Display this information\n");
}

int main(int argc, char *argv[])
{
    int opt;

    static struct option ops[] = {
        { "format", required_argument, NULL, 'f' },
        { "eat", required_argument, NULL, 'e' },
        { "help", no_argument, NULL, 'h'  }
    };
    while ((opt = getopt_long(argc, argv, "f:e:h", ops, NULL)) != -1)
    {
        switch(opt)
        {
        case 'f':
            formatted_text(optarg);
            return 0;
        case 'e':
            eat_trailing_space(optarg);
            return 0;
        case 'h':
            show_help();
            return 0;
        default:
            fprintf(stderr, "Try: %s --help\n", argv[0]);
            return -1;
        }
    }
    printf("Missing args. Try: %s --help\n", argv[0]);
    return 0;
}
