
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

struct format_officer_t
{
public:
    format_officer_t();
    ~format_officer_t();
    bool write_format_file(void);

    vector <string> line_vec;
    string format_file;
};

format_officer_t::format_officer_t()
{
    this->format_file = "format_";
}

format_officer_t::~format_officer_t() {  }

bool format_officer_t::write_format_file(void)
{
    bool result = false;
    fstream file_stream;
   
    if (this->line_vec.empty())
    {
        show_red("nothing to write\n");
        return result;
    }
    file_stream.open(this->format_file, ios::out);
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

static bool is_invalid_param(const char *path)
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
        show_red("target file too big, current limit: %u\n", MAXIMUM_FILE_SIZE);
        return result;
    }
    result = false;

    return result;
}

static int load_target_file(const char *path, format_officer_t *p_officer)
{
    int result = -1;
    filebuf fb;
    string line;

    if(fb.open(path, ios::in) == NULL)
    {
        show_red("open error: %s\n", strerror(errno));
        return result;
    }
    istream file_stream(&fb);

    while (getline(file_stream, line, '\n'))
    {
        p_officer->line_vec.push_back(line);
    }
    fb.close();

    result = 0;
    return result;
}

void deal_with_line(string &line)
{
    for (size_t i = 1; i < line.size(); i++)
    {
        if (!IS_CH(line[i])) {
            continue;
        }
        if (' ' != line[i - 1])
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

void format_each_line(format_officer_t *p_officer)
{
    size_t line_num = p_officer->line_vec.size();

    for (size_t i = 0; i < line_num; i++)
    {
        deal_with_line(p_officer->line_vec[i]);
    }
}

int formatted_text(const char *text_path)
{
    int result = -1;
    size_t pos;
    string name;
    format_officer_t *p_officer = new format_officer_t;

    if (is_invalid_param(text_path)) {
        goto done;
    }
    show("text path: ");
    show_green("%s\n", text_path);

    if (0 != load_target_file(text_path, p_officer))
    {
        show_red("error reading file into memory: %s\n", strerror(errno));
        goto done;
    }
    format_each_line(p_officer);

    name = text_path;

    if (string::npos != (pos = name.rfind('/')))
    {
        name.insert(pos + 1, p_officer->format_file);
    }
    else {
        name = p_officer->format_file + text_path;
    }
    p_officer->format_file = name;

    if (true == p_officer->write_format_file())
    {
        show("save to: ");
        show_green("%s\n", p_officer->format_file.c_str());
    }
done:
    if (p_officer)
    {
        delete p_officer;
        p_officer = NULL;
    }
    return result;
}

