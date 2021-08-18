
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

static int lock_file(int fd)
{
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    fl.l_pid = -1;

    return fcntl(fd, F_SETLK, &fl);
}

bool is_proc_running(const char *proc_name)
{
    char pid_file[PATH_MAX] = { 0 };
    snprintf(pid_file, PATH_MAX, "%s/.%s.pid", getenv("HOME"), proc_name);

    int fd = open(pid_file, O_CREAT|O_RDWR);
    if (fd < 0)
    {
        fprintf(stderr, "open %s failed %s\n", pid_file, strerror(errno));
        return false;
    }
    if (-1 == lock_file(fd))
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

    while (is_proc_running(proc_name))
    {
        sched_yield(); // give up the CPU (not essential)
    }
    return true;
}
