
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>     /* access ... */
#include <sys/stat.h>   /* stat */

#include <vector>
#include <string>
#include <fstream>      /* filebuf ... */
#include <iostream>

#include "formatted_text.h"

using namespace std;

/* ASCII 33~126 */
#define IS_CH(c) ((c >= '!' && c <= '~' && c != '(' && c != ')') ? true : false)


#define show(...) printf(__VA_ARGS__);

#define show_red(fmt, args...) \
{ \
    printf("\033[1;31;1m" fmt "\033[m",  ##args); \
}

#define show_green(fmt, args...) \
{ \
    printf("\033[1;32;1m" fmt "\033[m",  ##args); \
}

typedef enum
{
    FORMAT_ADD_SPACE,
    FORMAT_EAT_SPACE,

}fomat_type_t;

typedef void (*deal_with_line_t) (string &line);

struct format_officer_t
{
public:
    format_officer_t();
    ~format_officer_t();

    int load_target_file(const char *p_path);
    bool save_to_file(const char *p_path);

    vector <string> line_vec;
};

format_officer_t::format_officer_t() {  }

format_officer_t::~format_officer_t() {  }

int format_officer_t::load_target_file(const char *p_path)
{
    int result = -1;
    filebuf fb;
    string line;

    if(fb.open(p_path, ios::in) == NULL)
    {
        show_red("open error: %s\n", strerror(errno));
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

bool format_officer_t::save_to_file(const char *p_path)
{
    bool result = false;
    fstream file_stream;

    if (this->line_vec.empty())
    {
        show_red("nothing to write\n");
        return result;
    }
    file_stream.open(p_path, ios::out);
    if (!file_stream)
    {
        show_red("%s open error: %s\n", __func__, strerror(errno));
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

static bool is_invalid_path(const char *path)
{
    bool result = true;
    struct stat st;

    if (NULL == path)
    {
        show_red("the target path is empty\n");
        return result;
    }
    if (access(path, F_OK) == -1)
    {
        show_red("access target file failed: %s\n", path);
        return result;
    }
    stat(path, &st);
    if (S_ISDIR(st.st_mode))
    {
        show_red("the target path is a directory: %s\n", path);
        return result;
    }
    if (st.st_size > MAXIMUM_FILE_SIZE)
    {
        show_red("file size exceeds the limit: %u\n", MAXIMUM_FILE_SIZE);
        return result;
    }
    result = false;

    return result;
}

static void add_line_space(string &line)
{
    for (size_t i = 1; i < line.size(); i++)
    {
        if (!IS_CH(line[i]))
        {
            continue;
        }
        if (' ' != line[i - 1] && 1 != i)
        {
            line.insert(i, 1, ' ');
        }
        while (IS_CH(line[i]) && ' ' != line[i] && i < line.size())
        {
            i++;
        }
        if (' ' != line[i])
        {
            line.insert(i, 1, ' ');
        }
    }
}

void eat_trailing_space(string &line)
{
    for (int i = (int)line.length() - 1; i >= 0; i--)
    {
        if (' ' == line[i] || '\t' == line[i])
        {
            line.pop_back();
            continue;
        }
        break;
    }
}

static bool
format_each_line(format_officer_t *p_format, fomat_type_t type)
{
    size_t line_num = p_format->line_vec.size();
    deal_with_line_t line_func = NULL;

    switch (type)
    {
    case FORMAT_ADD_SPACE:
        line_func = add_line_space;
        break;
    case FORMAT_EAT_SPACE:
        line_func = eat_trailing_space;
        break;
    default:
        return false;
    }
    for (size_t i = 0; i < line_num; i++)
    {
        line_func(p_format->line_vec[i]);
    }
    return true;
}

static bool
read_and_backup(format_officer_t *p_format, const char *p_path)
{
    bool result = false;

    string backup_path = p_path;
    backup_path += ".bak";

    if (0 != p_format->load_target_file(p_path))
    {
        show_red("error reading file: %s\n", strerror(errno));
        return result;
    }
    /* backup */
    if (true == p_format->save_to_file(backup_path.c_str()))
    {
        show("backup to: ");
        show_green("\t%s\n", backup_path.c_str());

        result = true;
    }
    return result;
}

static bool formatted_text_inline(const char *p_path, fomat_type_t type)
{
    bool result = false;

    format_officer_t *p_format = new format_officer_t;

    if (is_invalid_path(p_path))
    {
        goto done;
    }
    show("input path: ");
    show_green("\t%s\n", p_path);

    if (false == read_and_backup(p_format, p_path))
    {
        goto done;
    }
    if (format_each_line(p_format, type) &&
        p_format->save_to_file(p_path))
    {
        show("save to: ");
        show_green("\t%s\n", p_path);
        result = true;
    }
done:
    if (p_format)
    {
        delete p_format;
        p_format = NULL;
    }
    return result;
}

bool formatted_text(const char *p_path)
{
    return formatted_text_inline(p_path, FORMAT_ADD_SPACE);
}

bool eat_trailing_space(const char *p_path)
{
    return formatted_text_inline(p_path, FORMAT_EAT_SPACE);
}
