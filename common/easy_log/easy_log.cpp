#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <stdarg.h> // va_list
#include <sys/time.h> // gettimeofday

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

#include "easy_log.h"

#define UTC_FORMAT_LEN  24

class easy_log
{
public:
    //for thread-safe singleton
    static easy_log* singleton()
    {
        pthread_once(&_once, easy_log::init);
        return _ins;
    }

    static void init()
    {
        while (!_ins) _ins = new easy_log();
    }
    bool init_path(const char *log_path, int level);

    int get_level() const { return _level; }

    void append(const char* lvl, const char* format, ...);

private:
    easy_log():_fp(NULL), _level(INFO) {  }

    bool decis_file();

    easy_log(const easy_log&);
    const easy_log& operator=(const easy_log&);

    FILE* _fp;
    char _log_path[PATH_MAX];
    int _level;

    static pthread_mutex_t _mutex;

    /* singleton */
    static easy_log *_ins;
    static pthread_once_t _once;
};

pthread_mutex_t easy_log::_mutex = PTHREAD_MUTEX_INITIALIZER;

easy_log* easy_log::_ins = NULL;
pthread_once_t easy_log::_once = PTHREAD_ONCE_INIT;

bool easy_log::init_path(const char *log_path, int level)
{
    bool result = true;
    pthread_mutex_lock(&_mutex);

    strncpy(_log_path, log_path, sizeof(_log_path));

    _fp = fopen(log_path, "w");

    if (NULL == _fp)
    {
        fprintf(stderr, "fopen '%s' failed %s\n",
                log_path, strerror(errno));
        result = false;
    }
    if (level > TRACE)
        level = TRACE;
    if (level < FATAL)
        level = FATAL;
    _level = level;

    pthread_mutex_unlock(&_mutex);
    return result;
}

void log_write(FILE *fp, const char *p_data, uint32_t len)
{
    uint32_t wt_len = (uint32_t)fwrite(p_data, 1, len, fp);
    if (wt_len != len)
    {
        fprintf(stderr,
                "write log to disk error, wt_len %u\n", wt_len);
    }
}

static void utc_format(char *utf_buf, uint32_t buf_len)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm curr;
    localtime_r((time_t*)&tv.tv_sec, &curr);

    snprintf(utf_buf, buf_len, "%d/%02d/%02d %02d:%02d:%02d",
             curr.tm_year + 1900, curr.tm_mon + 1, curr.tm_mday,
             curr.tm_hour, curr.tm_min, curr.tm_sec);
}

void easy_log::append(const char* lvl, const char* format, ...)
{
    if (!decis_file()) {
        return;
    }
    char log_line[LOG_LEN_LIMIT] = { 0 };
    char utc_buf[UTC_FORMAT_LEN] = { 0 };
    utc_format(utc_buf, UTC_FORMAT_LEN);

    int prev_len = snprintf(log_line, LOG_LEN_LIMIT, "%s %s ",
                            utc_buf, lvl);
    va_list arg_ptr;
    va_start(arg_ptr, format);

    int main_len = vsnprintf(log_line + prev_len,
                             (uint32_t)(LOG_LEN_LIMIT - prev_len),
                             format, arg_ptr);
    va_end(arg_ptr);

    uint32_t len = (uint32_t)(prev_len + main_len);

    pthread_mutex_lock(&_mutex);
    log_write(_fp, log_line, len);
    fflush(_fp);
    pthread_mutex_unlock(&_mutex);
}

static void utc_format_for_path(char *utf_buf, uint32_t buf_len)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm curr;
    localtime_r((time_t*)&tv.tv_sec, &curr);

    snprintf(utf_buf, buf_len, "%d-%02d-%02d_%02d:%02d:%02d",
             curr.tm_year + 1900, curr.tm_mon + 1, curr.tm_mday,
             curr.tm_hour, curr.tm_min, curr.tm_sec);
}

bool easy_log::decis_file()
{
    if (!_fp)
    {
        const char *log_path = _log_path;

        _fp = fopen(log_path, "w");
    }
    else if (ftell(_fp) >= LOG_USE_LIMIT)
    {
        fclose(_fp);
        char utc_buf[UTC_FORMAT_LEN] = { 0 };
        utc_format_for_path(utc_buf, UTC_FORMAT_LEN);
        char old_path[PATH_MAX] = { 0 };
        char new_path[PATH_MAX] = { 0 };
        //mv xxx to xxx_<time>
        sprintf(old_path, "%s", _log_path);
        sprintf(new_path, "%s_%s", _log_path, utc_buf);

        rename(old_path, new_path);

        _fp = fopen(old_path, "w");
    }
    return _fp != NULL;
}

void destroy_easy_log() {  }

bool init_easy_log(const char *log_path, int level)
{
    return easy_log::singleton()->init_path(log_path, level);
}

int log_get_level()
{
    return easy_log::singleton()->get_level();
}

void log_append(const char *level, const char *format, ...)
{
    va_list va;
    va_start(va, format);
    easy_log::singleton()->append(level, format, va);
    va_end(va);
}
