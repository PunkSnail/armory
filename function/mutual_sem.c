
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/ipc.h>
#include <sys/sem.h>  

#include "mutual_sem.h"


#ifdef _DEBUG
#define debug_print(...) { printf(__VA_ARGS__); }
#else
#define debug_print(...) { }
#endif /* _DEBUG */


typedef union
{
    int             val;
    struct semid_ds *buf;
    unsigned short  *array;

}mutual_sem_t;

void mutual_wait(int semid)  
{  
    struct sembuf sem;  
    sem.sem_num = 0;  
    sem.sem_op  = -1;
    sem.sem_flg = SEM_UNDO;
    
    if (-1 == semop(semid, &sem, 1)) {
        debug_print("%s failed: %s\n", __func__, strerror(errno));
    }  
}

void mutual_sig(int semid)  
{  
    struct sembuf sem;
    sem.sem_num = 0;  
    sem.sem_op  = 1;  
    sem.sem_flg = SEM_UNDO;  
    
    if (-1 == semop(semid, &sem, 1)) {
        debug_print("%s failed: %s\n", __func__, strerror(errno));
    }  
}

/* note:
 * IPC_PRIVATE 可用于父子进程通信 类似匿名管道 */
int create_producer_semid(void)
{  
    int result = -1;
    int semid = semget(MUTUAL_SEM_KEY, 1, IPC_CREAT | 0666);
    
    if (-1 == semid)
    {
        debug_print("%s semget error: %s\n", __func__, strerror(errno));
        return result;
    }  
    mutual_sem_t mutual_sem;
    memset(&mutual_sem, 0, sizeof(mutual_sem_t));
    
    mutual_sem.val = 1;
    result = semctl(semid, 0, SETVAL, mutual_sem);
    
    if (-1 == result)
    {  
        debug_print("%s semctl error: %s\n", __func__, strerror(errno));
        return result;
    }
    result = semid;
    return result;
}

void destroy_producer_semid(int semid)
{
    if (-1 == semctl(semid, 0, IPC_RMID)) {
        debug_print("%s semctl error: %s\n", __func__, strerror(errno));
    }
}

int create_consumer_semid(void)
{
    int result = semget(MUTUAL_SEM_KEY, 1, 0);
    
    if (-1 == result) {  
        debug_print("%s semget error: %s\n", __func__, strerror(errno));
    }  
    return result;
}
