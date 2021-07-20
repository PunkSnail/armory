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
    pthread_mutex_t lock;

} easy_log_t;

easy_log_t *g_log;

void destroy_easy_log()
{
    if (NULL != g_log)
    {
        if (NULL != g_log->fp)
        {
            fclose(g_log->fp);
        }
        pthread_mutex_destroy(&g_log->lock);

        free(g_log);
        g_log = NULL;
    }
}

// initialize or reset the easy log
bool init_easy_log(const char *log_path, log_level_t level)
{
    destroy_easy_log();

    g_log = (easy_log_t*)calloc(1, sizeof(easy_log_t));

    pthread_mutex_init(&g_log->lock, NULL);

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

log_level_t log_get_level()
{
    log_level_t result = FATAL;

    if (NULL != g_log)
    {
        result = g_log->level;
    }
    return result;
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
        utc_format_for_path(utc_buf, UTC_FORMAT_LEN);
        char old_path[PATH_MAX] = { 0 };
        char new_path[PATH_MAX] = { 0 };
#ifdef __linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
#endif
        // mv xxx to xxx_<time>
        snprintf(old_path, PATH_MAX, "%s", g_log->log_path);
        snprintf(new_path, PATH_MAX, "%s_%s", g_log->log_path, utc_buf);
#ifdef __linux__
#pragma GCC diagnostic pop
#endif
        rename(old_path, new_path);

        g_log->fp = fopen(old_path, "w");
    }
    return g_log->fp != NULL;
}

void log_append(const char *level, const char *format, ...)
{
    if (!log_append_check())
    {
        return;
    }
    char log_line[LOG_LEN_LIMIT] = { 0 };
    char utc_buf[UTC_FORMAT_LEN] = { 0 };

    utc_format(utc_buf, UTC_FORMAT_LEN);

    int prev_len = snprintf(log_line, LOG_LEN_LIMIT, "%s %s ",
                            utc_buf, level);
    va_list arg_ptr;
    va_start(arg_ptr, format);

    int content_len =
        vsnprintf(log_line + prev_len,
                  (uint32_t)(LOG_LEN_LIMIT - prev_len), format, arg_ptr);
    va_end(arg_ptr);

    uint32_t len = (uint32_t)(prev_len + content_len);

    pthread_mutex_lock(&g_log->lock);

    if (len != (uint32_t)fwrite(log_line, 1, len, g_log->fp))
    {
        fprintf(stderr, "write log to disk error %s\n", strerror(errno));
    }
    fflush(g_log->fp);

    pthread_mutex_unlock(&g_log->lock);
}
