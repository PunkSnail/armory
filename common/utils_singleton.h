#ifndef UTILS_SINGLETON_HH
#define UTILS_SINGLETON_HH

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
    // this is a write operation,
    // pid_file is the path to the file where the pid is stored
    bool is_proc_running(const char *pid_file);

    // kill the existing process, then replace it
    // Of course, the process is the process that has used this function
    bool proc_usurpation(const char *proc_name);

#ifdef __cplusplus
}
#endif

#endif // UTILS_SINGLETON_HH
