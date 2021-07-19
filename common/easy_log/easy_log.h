#ifndef EASY_LOG_HH
#define EASY_LOG_HH

#include <stdbool.h>
#include <stdint.h>

#define LOG_USE_LIMIT (1024 * 1024 * 100) // 100 MB
#define LOG_LEN_LIMIT (4 * 1024) // 4 KB

enum LOG_LEVEL
{
    FATAL = 1,
    ERROR,
    WARN,
    INFO,
    DEBUG,
    TRACE,
};

#ifdef __cplusplus
extern "C" {
#endif

    void destroy_log();

    bool carete_log(const char *log_path, int level);

    int log_get_level();

    void log_append(const char *level, const char *format, ...);

#ifdef __cplusplus
}
#endif

#define LOG_INIT(log_path, level) \
    carete_log(log_path, level);

#define LOG_TRACE(fmt, args...) \
    do \
{ \
    if (get_level() >= TRACE) \
    { \
        log_append("[TRACE]", fmt "\n", ##args); \
    } \
} while (0)

#define LOG_DEBUG(fmt, args...) \
    do \
{ \
    if (log_get_level() >= DEBUG) \
    { \
        log_append("[DEBUG]", fmt "\n", ##args); \
    } \
} while (0)

#define LOG_INFO(fmt, args...) \
    do \
{ \
    if (log_get_level() >= INFO) \
    { \
        log_append("[INFO]", fmt "\n", ##args); \
    } \
} while (0)

#define LOG_WARN(fmt, args...) \
    do \
{ \
    if (log_get_level() >= WARN) \
    { \
        log_append("[WARN]", fmt "\n", ##args); \
    } \
} while (0)

#define LOG_ERROR(fmt, args...) \
    do \
{ \
    if (log_get_level() >= ERROR) \
    { \
        log_append("[ERROR]", fmt "\n", ##args); \
    } \
} while (0)

#define LOG_FATAL(fmt, args...) \
    log_append("[FATAL]", fmt "\n", ##args);

#endif // EASY_LOG_HH
