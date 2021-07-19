#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h> // access
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
    int level;
    pthread_mutex_t lock;

} easy_log_t;

easy_log_t *g_log;

void destroy_log()
{
    if (NULL != g_log)
    {
        if (NULL != g_log->fp) {
            fclose(g_log->fp);
        }
        pthread_mutex_destroy(&g_log->lock);

        free(g_log);
        g_log = NULL;
    }
}

bool carete_log(const char *log_path, int level)
{
    g_log = (easy_log_t*)calloc(1, sizeof(easy_log_t));

    pthread_mutex_init(&g_log->lock, NULL);

    strncpy(g_log->log_path, log_path, sizeof(g_log->log_path));

    g_log->fp = fopen(log_path, "w");

    if (NULL == g_log->fp)
    {
        fprintf(stderr, "fopen '%s' failed %s\n",
                log_path, strerror(errno));
        destroy_log();
        return false;
    }
    if (level > TRACE)
        level = TRACE;
    if (level < FATAL)
        level = FATAL;
    g_log->level = level;

    return true;
}

int log_get_level() { return g_log->level; }

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

static bool decis_file()
{
    if (!g_log->fp)
    {
        g_log->fp = fopen(g_log->log_path, "w");
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
        //mv xxx to xxx_<time>
        sprintf(old_path, "%s", g_log->log_path);
        sprintf(new_path, "%s_%s", g_log->log_path, utc_buf);
#ifdef __linux__
#pragma GCC diagnostic pop
#endif
        rename(old_path, new_path);

        g_log->fp = fopen(old_path, "w");
    }
    return g_log->fp != NULL;
}

static void log_write(FILE *fp, const char *p_data, uint32_t len)
{
    if (len != (uint32_t)fwrite(p_data, 1, len, fp))
    {
        fprintf(stderr, "write log to disk error %s\n", strerror(errno));
    }
}

void log_append(const char *level, const char *format, ...)
{
    if (!decis_file()) {
        return;
    }
    char log_line[LOG_LEN_LIMIT] = { 0 };
    char utc_buf[UTC_FORMAT_LEN] = { 0 };

    utc_format(utc_buf, UTC_FORMAT_LEN);

    int prev_len = snprintf(log_line, LOG_LEN_LIMIT, "%s %s ",
                            utc_buf, level);
    va_list arg_ptr;
    va_start(arg_ptr, format);

    int main_len = vsnprintf(log_line + prev_len,
                             (uint32_t)(LOG_LEN_LIMIT - prev_len),
                             format, arg_ptr);
    va_end(arg_ptr);

    uint32_t len = (uint32_t)(prev_len + main_len);

    pthread_mutex_lock(&g_log->lock);

    log_write(g_log->fp, log_line, len);
    fflush(g_log->fp);

    pthread_mutex_unlock(&g_log->lock);
}
