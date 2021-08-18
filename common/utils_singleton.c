
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h> // kill
#include <unistd.h> // read, write
#include <fcntl.h>
#include <pthread.h> // sched_yield

#include "utils_singleton.h"

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

bool is_proc_running(const char *pid_file)
{
    int fd = open(pid_file, O_CREAT|O_RDWR);
    if (fd < 0)
    {
        fprintf(stderr, "open %s failed %s\n", pid_file, strerror(errno));
        return false;
    }
    struct flock lock = {
        .l_type = F_WRLCK,
        .l_whence = SEEK_SET,
        .l_start = 0, // the offset of the start of the region
        .l_len = 0, // 0 means to the end of the file
        .l_pid = -1, // -1 means the lock isn't associated with a process
    };
    if (-1 == fcntl(fd, F_SETLK, &lock))
    {
        close(fd);
        return true;
    }
    ftruncate(fd, 0);
    dprintf(fd, "%ld", (long)getpid());
    // do not close the file descriptor
    fsync(fd);

    return false;
}

// kill the existing process, then replace it
bool proc_usurpation(const char *proc_name)
{
    char buf[32] = { 0 };
    char pid_file[PATH_MAX] = { 0 };

    snprintf(pid_file, PATH_MAX, "%s/.%s.pid", getenv("HOME"), proc_name);
    // check the permissions by the way
    int fd = open(pid_file, O_CREAT|O_RDWR);
    if (fd < 0)
    {
        fprintf(stderr, "open %s failed %s\n", pid_file, strerror(errno));
        return false;
    }
    if (read(fd, buf, sizeof(buf)) > 0)
    {
        kill((pid_t)atol(buf), SIGKILL);
    }
    close(fd);

    while (is_proc_running(pid_file))
    {
        sched_yield(); // give up the CPU (not essential)
    }
    return true;
}
