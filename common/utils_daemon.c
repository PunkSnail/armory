
#include <stdlib.h>     /* exit ... */
#include <unistd.h>     /* fork ... */
#include <signal.h>     /* SIGHUP ... */
#include <stdbool.h>    /* bool */
#include <fcntl.h>      /* O_RDONLY ... */

#include "utils_daemon.h"

static int ignore_signal(int signal, bool is_once)
{
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));

    act.sa_handler = SIG_IGN;
    sigemptyset(&(act.sa_mask));
    act.sa_flags |= (int)((true == is_once) ? SA_RESETHAND : 0);

    return sigaction(signal, &act, NULL);
}

static int become_child(void)
{
    int pid = (int)fork();
    if (0 < pid)
    {
        exit(0);
    }
    return pid;
}

int become_daemon(void)
{
    int result = -1;
    if (0 == become_child() && 0 < setsid()
        && 0 == ignore_signal(SIGHUP, false)
        && 0 == become_child() )
    {
        result = 0;
    }
    int temp = close(STDIN_FILENO);
    temp |= close(STDOUT_FILENO);
    temp |= close(STDERR_FILENO);

    if (0 == result && 0 == temp
        && STDIN_FILENO == open("/dev/null", O_RDONLY)
        && STDOUT_FILENO == open("/dev/null", O_WRONLY)
        && STDERR_FILENO == open("/dev/null", O_WRONLY) )
    {
        result = 0;
    }
    else {
        result = -1;
    }
    return result;
}

