
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>     /* va_start */
#include <errno.h>
#include <unistd.h>     /* access ... */
#include <sys/stat.h>   /* stat */
#include <elf.h>        /* Elf64_Ehdr */

#include <vector>
#include <string>
#include <fstream>      /* filebuf ... */
#include <iostream>

#include "formatted_text.h"

using namespace std;

/* ASCII 33~126 */
#define IS_CH(c) (c >= '!' && c <= '~' && c != '(' && c != ')')


typedef bool (*deal_with_line_t) (string &line, bool is_check);

struct format_officer
{
public:
    format_officer();
    ~format_officer();

    int load_target_file(const char *p_path);
    bool save_to_file(const char *p_path);

    void show(const char *fmt, ...);
    void show_red(const char *fmt, ...);
    void show_green(const char *fmt, ...);

    bool is_invalid_path(const char *path);

    vector <deal_with_line_t> func_vec;
    vector <string> line_vec;
    bool backup;
    bool dispaly;

    char fmt[1024];
};

format_officer::format_officer()
{
    this->backup = true;
    this->dispaly = true;
}

format_officer::~format_officer() {  }

void format_officer::show(const char *fmt, ...)
{
	va_list a_list;

    if (false == this->dispaly)
    {
        return;
    }
    va_start(a_list, fmt);

    vdprintf(STDOUT_FILENO, fmt, a_list);

	va_end(a_list);
}

void format_officer::show_red(const char *fmt, ...)
{
    va_list a_list;

    if (false == this->dispaly)
    {
        return;
    }
    va_start(a_list, fmt);

    snprintf(this->fmt, sizeof(this->fmt), "\033[1;31;1m%s\033[m", fmt);

    vdprintf(STDOUT_FILENO, this->fmt, a_list);

    va_end(a_list);
}

void format_officer::show_green(const char *fmt, ...)
{
    va_list a_list;
    if (false == this->dispaly)
    {
        return;
    }
    va_start(a_list, fmt);

    snprintf(this->fmt, sizeof(this->fmt), "\033[1;32;1m%s\033[m", fmt);

    vdprintf(STDOUT_FILENO, this->fmt, a_list);

    va_end(a_list);
}

int format_officer::load_target_file(const char *p_path)
{
    int result = -1;
    filebuf fb;
    string line;

    if(fb.open(p_path, ios::in) == NULL)
    {
        this->show_red("open error: %s\n", strerror(errno));
        return result;
    }
    istream file_stream(&fb);

    while (getline(file_stream, line, '\n'))
    {
        this->line_vec.push_back(line);
    }
    fb.close();

    result = 0;
    return result;
}

bool format_officer::save_to_file(const char *p_path)
{
    bool result = false;
    fstream file_stream;

    if (this->line_vec.empty())
    {
        this->show_red("nothing to write\n");
        return result;
    }
    file_stream.open(p_path, ios::out);
    if (!file_stream)
    {
        this->show_red("%s open error: %s\n", p_path, strerror(errno));
        return result;
    }
    for (size_t i = 0; i < this->line_vec.size(); i++)
    {
        file_stream << this->line_vec[i] << "\n";
    }
    file_stream.close();
    result = true;

    return result;
}

static bool file_is_elf(const char *path)
{
    bool result = false;

    Elf64_Ehdr elf_head;

    FILE *rfp = fopen(path, "rb");

    if (NULL == rfp || 0 == fread(&elf_head, 1, sizeof(Elf64_Ehdr), rfp))
    {
        // printf("%s: %s\n", __func__, strerror(errno));
        exit(errno);
    }
    if (elf_head.e_ident[0] == ELFMAG0 &&
        elf_head.e_ident[1] == ELFMAG1 &&
        elf_head.e_ident[2] == ELFMAG2 &&
        elf_head.e_ident[3] == ELFMAG3)
    {
        result = true;
    }
    fclose(rfp);

    return result;
}

bool format_officer::is_invalid_path(const char *path)
{
    bool result = true;
    struct stat st;

    if (NULL == path)
    {
        this->show_red("the target path is empty\n");
        return result;
    }
    if (access(path, F_OK | R_OK | W_OK) == -1)
    {
        show_red("%s: %s\n", path, strerror(errno));
        return result;
    }
    stat(path, &st);

    if (S_ISDIR(st.st_mode))
    {
        show_red("the target path is a directory: %s\n", path);
    }
    else if (!S_ISREG(st.st_mode))
    {
        show_red("the file is not a regular file\n");
    }
    else if (st.st_size > MAXIMUM_FILE_SIZE)
    {
        show_red("file size exceeds the limit: %u\n", MAXIMUM_FILE_SIZE);
    }
    else if ((size_t)st.st_size > sizeof(Elf64_Ehdr) && file_is_elf(path))
    {
        show_red("this file type is not supported\n");
    }
    else
    {
        result = false;
    }
    return result;
}

format_officer_t *format_officer_create()
{
    format_officer_t *p_format = new format_officer_t;

    return p_format;
}

static bool add_line_space(string &line, bool is_check)
{
    bool need_format = false;

    for (size_t i = 1; i < line.size(); i++)
    {
        if (!IS_CH(line[i]))
        {
            continue;
        }
        if (' ' != line[i - 1] && 1 != i)
        {
            if (is_check)
            {
                need_format = true;
                goto check_end;
            }
            line.insert(i, 1, ' ');
        }
        while (IS_CH(line[i]) && ' ' != line[i] && i < line.size())
        {
            i++;
        }
        if (' ' != line[i])
        {
            if (is_check)
            {
                need_format = true;
                goto check_end;
            }
            line.insert(i, 1, ' ');
        }
    }
check_end:
    return need_format;
}

static bool eat_trailing_space(string &line, bool is_check)
{
    bool need_format = false;

    for (int i = (int)line.length() - 1; i >= 0; i--)
    {
        if (' ' == line[i] || '\t' == line[i])
        {
            if (is_check)
            {
                need_format = true;
                goto check_end;
            }
            line.pop_back();
            continue;
        }
        break;
    }
check_end:
    return need_format;
}

static void
get_format_func_array(format_officer_t *p_format, fomat_flags_t flags)
{
    p_format->func_vec.clear();

    if (flags & FORMAT_ADD_SPACE)
    {
        p_format->func_vec.push_back(add_line_space);
    }
    if (flags & FORMAT_EAT_SPACE)
    {
        p_format->func_vec.push_back(eat_trailing_space);
    }
}

static bool
format_check_each_line(format_officer_t *p_format, bool is_check)
{
    int line_num = (int)p_format->line_vec.size();
    int func_num = (int)p_format->func_vec.size();

    deal_with_line_t func = NULL;
    bool result = false;

    if (0 == func_num)
    {
        return result;
    }
    for (int i = 0; i < line_num; i++)
    {
        for (int j = 0; j < func_num; j++)
        {
            func = p_format->func_vec[j];
            result = func(p_format->line_vec[i], is_check);

            if (result && is_check)
            {
                return result;
            }
        }
    }
    return result;
}

bool formatting_check(format_officer_t *p_format,
                      const char *p_path, fomat_flags_t flags)
{
    bool result = false;

    if (0 == (flags & FORMAT_FMT_CHECK) ||
        p_format->is_invalid_path(p_path))
    {
        return result;
    }
    p_format->dispaly = (flags & FORMAT_UNDISPLAY) ? false : true;

    if (0 != p_format->load_target_file(p_path))
    {
        p_format->show_red("error reading file\n");
        return result;
    }
    get_format_func_array(p_format, flags);

    if (0 == p_format->func_vec.size())
    {
        p_format->show_red("No formatting type specified\n");
        return result;
    }
    result = format_check_each_line(p_format, true);

    if (result)
    {
        p_format->show_red("\"%s\" needs to be formatted\n", p_path);
    }
    else
    {
        p_format->show_green("\"%s\" no formatting required\n", p_path);
    }
    return result;
}

static bool
read_and_backup(format_officer_t *p_format, const char *p_path)
{
    bool result = false;

    string backup_path = p_path;
    backup_path += ".bak";

    if (0 != p_format->load_target_file(p_path))
    {
        p_format->show_red("error reading file\n");
        return result;
    }
    /* backup */
    if (false == p_format->backup)
    {
        result = true;
    }
    else if (p_format->save_to_file(backup_path.c_str()))
    {
        p_format->show("backup to: ");
        p_format->show_green("\t%s\n", backup_path.c_str());

        result = true;
    }
    return result;
}

bool formatting_file(format_officer_t *p_format, const char *p_path,
                     const char *p_save_path, fomat_flags_t flags)
{
    bool result = false;

    p_format->backup = (flags & FORMAT_TO_BACKUP) ? true : false;
    p_format->dispaly = (flags & FORMAT_UNDISPLAY) ? false : true;

    if (FORMAT_UNDEFINED == flags)
    {
        p_format->show_red("No formatting type specified\n");
        return result;
    }
    if (p_format->is_invalid_path(p_path))
    {
        return result;
    }
    p_format->show("input path: ");
    p_format->show_green("\t%s\n", p_path);

    if (NULL == p_save_path)
    {
        p_save_path = p_path;
    }
    if (false == read_and_backup(p_format, p_path))
    {
        return result;
    }
    get_format_func_array(p_format, flags);

    format_check_each_line(p_format, false);

    if (p_format->save_to_file(p_save_path))
    {
        p_format->show("save to: ");
        p_format->show_green("\t%s\n", p_save_path);
        result = true;
    }
    return result;
}

void format_officer_destroy(format_officer_t *p_format)
{
    if (p_format)
    {
        delete p_format;
        p_format = NULL;
    }
}

