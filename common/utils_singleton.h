#ifndef UTILS_SINGLETON_HH
#define UTILS_SINGLETON_HH

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    bool is_proc_running(const char *proc_name);

    // kill the existing process, then replace it
    bool proc_usurpation(const char *proc_name);

#ifdef __cplusplus
}
#endif

#endif // UTILS_SINGLETON_HH
