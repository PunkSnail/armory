#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "mutual_sem.h"

typedef union
{
    int             val;
    struct semid_ds *buf;
    unsigned short  *array;

} mutual_sem_t;

int mutual_lock(int semid)
{
    struct sembuf sem;
    sem.sem_num = 0;
    sem.sem_op  = -1;
    sem.sem_flg = SEM_UNDO;

    return semop(semid, &sem, 1);
}

int mutual_unlock(int semid)
{
    struct sembuf sem;
    sem.sem_num = 0;
    sem.sem_op  = 1;
    sem.sem_flg = SEM_UNDO;

    return semop(semid, &sem, 1);
}

int create_producer_semid(key_t key)
{
    int result = -1;
    // 666 means that everyone can rw
    int semid = semget(key, 1, IPC_CREAT | 0666);

    if (-1 == semid)
    {
        return result;
    }
    mutual_sem_t mutual_sem;
    memset(&mutual_sem, 0, sizeof(mutual_sem_t));

    mutual_sem.val = 1;
    result = semctl(semid, 0, SETVAL, mutual_sem);

    if (-1 == result)
    {
        return result;
    }
    result = semid;
    return result;
}

int destroy_producer_semid(int semid)
{
    return semctl(semid, 0, IPC_RMID);
}

int attach_consumer_semid(key_t key)
{
    return semget(key, 1, 0);
}
