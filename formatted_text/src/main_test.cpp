#include <stdio.h>
#include <getopt.h> /* getopt_long */

#include "formatted_text.h"

void show_help(void)
{
    printf("Options:\n"
           "-f <file>       Format the specified file (add spaces)\n"
           "-e <file>       Eat the trailing space in the <file>\n"
           "-o <file>       Place the output into <file>\n"
           "-b, --backup    Save original file to <file>.bak\n"
           "-h, --help      Display this information\n");
}

int main(int argc, char *argv[])
{
    int opt;
    bool backup = false;
    const char *p_path = NULL;
    const char *p_save_path = NULL;
    fomat_type_t type = FORMAT_UNDEFINED;

    static struct option ops[] = {
        { "format", required_argument, NULL, 'f' },
        { "eat", required_argument, NULL, 'e' },
        { "output", required_argument, NULL, 'o' },
        { "backup", no_argument, NULL, 'b' },
        { "help", no_argument, NULL, 'h'  }
    };
    while ((opt = getopt_long(argc, argv, "f:e:o:bh", ops, NULL)) != -1)
    {
        switch(opt)
        {
        case 'f':
            p_path = optarg;
            type = FORMAT_ADD_SPACE;
            break;
        case 'e':
            p_path = optarg;
            type = FORMAT_EAT_SPACE;
            break;
        case 'o':
            p_save_path = optarg;
            break;
        case 'b':
            backup = true;
            break;
        case 'h':
            show_help();
            return 0;
        default:
            fprintf(stderr, "Try: %s --help\n", argv[0]);
            return -1;
        }
    }
    formatting_file(p_path, p_save_path, type, backup);

    if (1 == argc)
    {
        printf("Missing args. Try: %s --help\n", argv[0]);
    }
    return 0;
}
