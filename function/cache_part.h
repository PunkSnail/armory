
#ifndef CACHE_PART_HHH
#define CACHE_PART_HHH

#include <stdint.h>

/* note:
 * 大于 MEMORY_CACHE_LEN 缓存到文件 
 * 小于则缓存到内存(外部提供, 也许是共享内存)
 * 通常用于网络 IO
 */
#define MEMORY_CACHE_LEN 10485760   /* 10mb */ 

/* cache path format: dir/punk_cache_123 */
#define CACHE_NAME_HEAD "punk_cache_"

#ifndef MAX_PATH
#define MAX_PATH 1024
#endif

typedef struct private_cache private_cache_t;

typedef struct 
{
    uint64_t    cache_len;
    uint32_t    last_len;  // The last read len
   
    char        *buf;
    
    char        path[MAX_PATH];
    uint32_t    index_name; // to splice file path

    private_cache_t *p_priv;

}cache_t;

/* if "cache_dir" is NULL cache_file in current directory */
cache_t *init_cache(const char *cache_dir);

/* note:
 * "cache_buf" minimum limit is MEMORY_CACHE_LEN bytes */
int create_cache(cache_t *p_cache, char *cache_buf, uint64_t packet_len);

/* return: finish 0    unfinished 1    fail -1 */
int cache_recv_data(cache_t *p_cache, const char *p_data, uint32_t data_len);

/* cache file need outside delete */
void leave_cache(cache_t *p_cache);

void destroy_cache(cache_t *p_cache);

#endif /* CACHE_PART_HHH */

