#ifndef MUTUAL_SEM_HHH
#define MUTUAL_SEM_HHH

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

// If successful, the return value will be a nonnegative integer;
// otherwise, -1 is returned, with errno indicating the error.

int create_producer_semid(key_t key);

int attach_consumer_semid(key_t key);

// If successful, return 0;
// otherwise, -1 is returned, with errno indicating the error.

int mutual_lock(int semid);

int mutual_unlock(int semid);

int destroy_producer_semid(int semid);

#ifdef __cplusplus
}
#endif

#endif /* MUTUAL_SEM_HHH */
