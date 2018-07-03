
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/ipc.h>
#include <sys/sem.h>  

#include "mutual_sem.h"
 
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
#ifdef _DEBUG
        perror("mutual_wait failed");
#endif
    }  
}

void mutual_sig(int semid)  
{  
    struct sembuf sem;
    sem.sem_num = 0;  
    sem.sem_op  = 1;  
    sem.sem_flg = SEM_UNDO;  
    
    if (-1 == semop(semid, &sem, 1)) {
#ifdef _DEBUG        
        perror("mutual_sig failed");  
#endif
    }  
}

/* note:
 * IPC_PRIVATE 可用于父子进程通信 类似匿名管道 */
int create_producer_semid(void)
{  
    int result = -1;
    int semid = semget(MUTUAL_SEM_KEY, 1, IPC_CREAT | 0666);
    
    if (-1 == semid) {
#ifdef _DEBUG
        perror("semget error");  
#endif
        return result;
    }  
    mutual_sem_t mutual_sem;
    memset(&mutual_sem, 0, sizeof(mutual_sem_t));
#if 1    
    mutual_sem.val = 1;
    result = semctl(semid, 0, SETVAL, mutual_sem);
#else
    mutual_sem.val = 1;
    if (semctl(semid, 0, SETVAL, mutual_sem) < 0) {
        perror("semctl failed");  
    }  
    mutual_sem.val = 0;
    if (semctl(semid, 1, SETVAL, mutual_sem) < 0) {  
        perror("semctl failed");  
        exit(1);  
    }  
    mutual_sem.val = 1;
    if (semctl(semid, 2, SETVAL, mutual_sem) < 0) {
        perror("semctl failed");  
    }  
#endif
    if (-1 == result) {  
#ifdef _DEBUG
        perror("semctl error");  
#endif
        return result;
    }
    result = semid;
    return result;
}

void destroy_producer_semid(int semid)
{
    if (-1 == semctl(semid, 0, IPC_RMID)) {
#ifdef _DEBUG
        perror("semctl error");  
#endif
    }
}

int create_consumer_semid(void)
{
    int result = semget(MUTUAL_SEM_KEY, 1, 0);
    
    if (-1 == result) {  
#ifdef _DEBUG
        perror("semget error");  
#endif
    }  
    return result;
}
