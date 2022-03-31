#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdarg.h> // va_list
#include <sys/time.h> // gettimeofday

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

#include "easy_log.h"

#define UTC_FORMAT_LEN  24

typedef struct
{
    FILE *fp;
    char log_path[PATH_MAX];
    log_level_t level;
    bool enable_lock;
    pthread_mutex_t lock;
    char log_buf[LOG_LEN_LIMIT];

} easy_log_t;

easy_log_t *g_log;

#define LOG_INTERNAL_LOCK() \
{ \
    if (g_log->enable_lock) { \
        pthread_mutex_lock(&g_log->lock); \
    } \
}

#define LOG_INTERNAL_UNLOCK() \
{ \
    if (g_log->enable_lock) { \
        pthread_mutex_unlock(&g_log->lock); \
    } \
}

void destroy_easy_log()
{
    if (g_log)
    {
        if (g_log->fp)
        {
            fclose(g_log->fp);
        }
        if (g_log->enable_lock)
        {
            pthread_mutex_destroy(&g_log->lock);
        }
        free(g_log);
        g_log = NULL;
    }
}

// initialize or reset the easy log
bool init_easy_log(const char *log_path,
                   log_level_t level, bool enable_lock)
{
    destroy_easy_log();

    g_log = (easy_log_t*)calloc(1, sizeof(easy_log_t));

    if (NULL == g_log)
    {
        fprintf(stderr, "calloc failed %s\n", strerror(errno));
        return false;
    }
    if (enable_lock)
    {
        g_log->enable_lock = true;
        pthread_mutex_init(&g_log->lock, NULL);
    }
    strncpy(g_log->log_path, log_path, sizeof(g_log->log_path));

    g_log->fp = fopen(log_path, "ab+"); // append or create a log file

    if (NULL == g_log->fp)
    {
        fprintf(stderr, "fopen '%s' failed %s\n",
                log_path, strerror(errno));
        destroy_easy_log();
        return false;
    }
    g_log->level = level;

    return true;
}

log_level_t log_get_level(void)
{
    return g_log ? g_log->level : LOG_LEVEL_FATAL;
}

static void utc_format(char *utf_buf, uint32_t buf_len, bool path_suffix)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm curr;
    localtime_r((time_t*)&tv.tv_sec, &curr);

    if (path_suffix)
    {
        snprintf(utf_buf, buf_len, "%d-%02d-%02d_%02d:%02d:%02d",
                 curr.tm_year + 1900, curr.tm_mon + 1, curr.tm_mday,
                 curr.tm_hour, curr.tm_min, curr.tm_sec);
        return;
    }
    snprintf(utf_buf, buf_len, "%d/%02d/%02d %02d:%02d:%02d",
             curr.tm_year + 1900, curr.tm_mon + 1, curr.tm_mday,
             curr.tm_hour, curr.tm_min, curr.tm_sec);
}

static bool log_append_check()
{
    if (NULL == g_log)
    {
        return false;
    }
    else if (ftell(g_log->fp) >= LOG_USE_LIMIT)
    {
        fclose(g_log->fp);
        char utc_buf[UTC_FORMAT_LEN] = { 0 };
        utc_format(utc_buf, UTC_FORMAT_LEN, true);

        // TMPBUF_LEN avoids a silly warning
#define TMPBUF_LEN PATH_MAX + UTC_FORMAT_LEN
        char old_path[TMPBUF_LEN] = { 0 };
        char new_path[TMPBUF_LEN] = { 0 };
        // mv xxx to xxx_<time>
        snprintf(old_path, TMPBUF_LEN, "%s", g_log->log_path);
        snprintf(new_path, TMPBUF_LEN, "%s_%s", g_log->log_path, utc_buf);

        rename(old_path, new_path);

        g_log->fp = fopen(old_path, "w");
    }
    return g_log->fp != NULL;
}

bool log_append(const char *level, const char *module,
                const char *format, ...)
{
    bool result = false;

    va_list arg_ptr;
    uint32_t write_len = 0;
    int prev_len = 0, content_len = 0;

    char *buf = NULL;
    char utc_buf[UTC_FORMAT_LEN] = { 0 };
    utc_format(utc_buf, UTC_FORMAT_LEN, false);

    LOG_INTERNAL_LOCK();

    if (!log_append_check())
    {
        goto append_end;
    }
    buf = g_log->log_buf;

    prev_len = (NULL == module) ?
        snprintf(buf, LOG_LEN_LIMIT, "%s %s ", utc_buf, level) :
        snprintf(buf, LOG_LEN_LIMIT,
                 "%s %s [%s] ", utc_buf, level, module);
    if (prev_len < 0)
    {
        fprintf(stderr, "snprintf failed %s\n", strerror(errno));
        goto append_end;
    }
    va_start(arg_ptr, format);

    content_len = vsnprintf(buf + prev_len, (uint32_t)
                            (LOG_LEN_LIMIT - prev_len), format, arg_ptr);
    if (content_len < 0)
    {
        fprintf(stderr, "vsnprintf failed %s\n", strerror(errno));
        goto append_end;
    }
    va_end(arg_ptr);

    write_len = (uint32_t)(prev_len + content_len);

    if (write_len != (uint32_t)fwrite(buf, 1, write_len, g_log->fp))
    {
        fprintf(stderr, "write log to disk error %s\n", strerror(errno));
    } else {
        result = true; // narmal case
    }
    fflush(g_log->fp);
append_end:
    LOG_INTERNAL_UNLOCK();
    return result;
}
