#ifndef EASY_LOG_HH
#define EASY_LOG_HH

#include <stdbool.h>
#include <stdint.h>

#define LOG_USE_LIMIT (1024 * 1024 * 100) // 100 MB
#define LOG_LEN_LIMIT (4 * 1024) // 4 KB

typedef enum
{
    LOG_LEVEL_FATAL = 1,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE,
}log_level_t;

#ifdef __cplusplus
extern "C" {
#endif
    // initialize or reset the easy log
    bool init_easy_log(const char *log_path,
                       log_level_t level, bool enable_lock);

    log_level_t log_get_level(void);
    // thread-safe, if the lock was enabled
    bool log_append(const char *level, const char *module,
                    const char *format, ...);

    void destroy_easy_log();

#ifdef __cplusplus
}
#endif

// default for single thread
#define LOG_INIT(log_path, level) init_easy_log(log_path, level, false);


#define MODLOG_TRACE(module, fmt, args...) \
{ \
    if (log_get_level() >= LOG_LEVEL_TRACE) \
    { \
        log_append("[TRACE]", module, fmt "\n", ##args); \
    } \
}

#define LOG_TRACE(fmt, args...) MODLOG_TRACE(NULL, fmt, ##args)


#define MODLOG_DEBUG(module, fmt, args...) \
{ \
    if (log_get_level() >= LOG_LEVEL_DEBUG) \
    { \
        log_append("[DEBUG]", module, fmt "\n", ##args); \
    } \
}

#define LOG_DEBUG(fmt, args...) MODLOG_DEBUG(NULL, fmt, ##args)

#define MODLOG_INFO(module, fmt, args...) \
{ \
    if (log_get_level() >= LOG_LEVEL_INFO) \
    { \
        log_append("[INFO]", module, fmt "\n", ##args); \
    } \
}

#define LOG_INFO(fmt, args...) MODLOG_INFO(NULL, fmt, ##args)


#define MODLOG_WARN(module, fmt, args...) \
{ \
    if (log_get_level() >= LOG_LEVEL_WARN) \
    { \
        log_append("[WARN]", module, fmt "\n", ##args); \
    } \
}

#define LOG_WARN(fmt, args...) MODLOG_WARN(NULL, fmt, ##args)


#define MODLOG_ERROR(module, fmt, args...) \
{ \
    if (log_get_level() >= LOG_LEVEL_ERROR) \
    { \
        log_append("[ERROR]", NULL, fmt "\n", ##args); \
    } \
}

#define LOG_ERROR(fmt, args...) MODLOG_ERROR(NULL, fmt, ##args)


#define MODLOG_FATAL(module, fmt, args...) \
    log_append("[FATAL]", module, fmt "\n", ##args);

#define LOG_FATAL(fmt, args...) MODLOG_FATAL(NULL, fmt, ##args)

#endif // EASY_LOG_HH
