
#ifndef MUTUAL_SEM_HHH
#define MUTUAL_SEM_HHH

#define MUTUAL_SEM_KEY 0x6b6e7570 /* punk */

/* note:
 * 用于进程同步 通常与共享内存搭配使用
 * */

#ifdef __cpluscplus
extern "C" {
#endif

/* producer only 
 * return: succ semid   fail -1 */
int create_producer_semid(void);

void destroy_producer_semid(int semid);

/* consumer only 
 * return: succ semid   fail -1 */
int create_consumer_semid(void);

/* common part */
void mutual_wait(int semid);

void mutual_sig(int semid);

#ifdef __cpluscplus
}
#endif

#endif /* MUTUAL_SEM_HHH */
