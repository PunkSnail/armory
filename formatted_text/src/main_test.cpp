#include <stdio.h>
#include <getopt.h> /* getopt_long */

#include "formatted_text.h"

void show_help(void)
{
    printf("Options:\n"
           "-f <file>       Format the specified <file>\n"
           "-o <file>       Place the output into <file>\n"
           "-e, --eat       Eat the spaces at the end of each line\n"
           "-i, --insert    Insert spaces between text\n"
           "-c, --check     Check if formatting is required\n"
           "-b, --backup    Save original <file> to <file>.bak\n"
           "-u, --undisplay The process is not displayed\n"
           "-h, --help      Display this information\n");
}

int main(int argc, char *argv[])
{
    int opt;
    const char *p_path = NULL;
    const char *p_save_path = NULL;
    fomat_flags_t flags = FORMAT_UNDEFINED;

    static struct option ops[] = {
        { "file", required_argument, NULL, 'f' },
        { "output", required_argument, NULL, 'o' },
        { "eat", no_argument, NULL, 'e' },
        { "insert", no_argument, NULL, 'i' },
        { "check", no_argument, NULL, 'c' },
        { "backup", no_argument, NULL, 'b' },
        { "undisplay", no_argument, NULL, 'u' },
        { "help", no_argument, NULL, 'h' }
    };
    while ((opt = getopt_long(argc, argv, "f:o:eicbuh", ops, NULL)) != -1)
    {
        switch(opt)
        {
        case 'f':
            p_path = optarg;
            break;
        case 'o':
            p_save_path = optarg;
            break;
        case 'e':
            flags |= FORMAT_EAT_SPACE;
            break;
        case 'i':
            flags |= FORMAT_ADD_SPACE;
            break;
        case 'c':
            flags |= FORMAT_FMT_CHECK;
            break;
        case 'b':
            flags |= FORMAT_TO_BACKUP;
            break;
        case 'u':
            flags |= FORMAT_UNDISPLAY;
            break;
        case 'h':
            show_help();
            return 0;
        default:
            fprintf(stderr, "Try: %s --help\n", argv[0]);
            return -1;
        }
    }
    if (NULL == p_path)
    {
        printf("Missing args. Try: %s --help\n", argv[0]);
        return -1;
    }
    format_officer_t *p_format = format_officer_create();

    if (flags & FORMAT_FMT_CHECK)
    {
        formatting_check(p_format, p_path, flags);
    }
    else
    {
        formatting_file(p_format, p_path, p_save_path, flags);
    }
    format_officer_destroy(p_format);

    return 0;
}
