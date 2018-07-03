#define _CRT_SECURE_NO_WARNINGS

#include "cache_part.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <io.h> /* remove */
#elif defined __linux__
#include <unistd.h>
#include <sys/time.h>
#endif

struct private_cache
{
    uint64_t    packet_len;
    uint64_t    wlen;       // write length

    FILE        *wfp;
    uint32_t    priv_index; // for cache file
    char        dir[MAX_PATH];
};

cache_t *init_cache(const char *cache_dir)
{
    cache_t *p_cache = (cache_t*)calloc(1, sizeof(cache_t));
 
    if (p_cache)
    {
        p_cache->p_priv = (private_cache_t*)calloc(1, sizeof(private_cache_t));

        if (cache_dir)
        {
            size_t dir_index = strlen(cache_dir) - 1;
            
            if('/' == cache_dir[dir_index] || '\\' == cache_dir[dir_index]) {
                strncpy(p_cache->p_priv->dir, cache_dir, strlen(cache_dir));
            }
        }
        else {
            strncpy(p_cache->p_priv->dir, ".", 1);
        }
    }
    return p_cache;
}

static int create_cache_file(cache_t *p_cache)
{
    int result = -1;

    snprintf(p_cache->path, MAX_PATH, "%s%s%u", 
             p_cache->p_priv->dir, CACHE_NAME_HEAD, ++p_cache->p_priv->priv_index);

    p_cache->index_name = p_cache->p_priv->priv_index;
    /* cache file deleted by handle */
    p_cache->p_priv->wfp = fopen(p_cache->path, "wb");

    if (NULL == p_cache->p_priv->wfp) {
#ifdef _DEBUG
        perror("fopen error");
#endif
        return result;
    }
    result = 0;
    return result;
}

int create_cache(cache_t *p_cache, char *cache_buf, uint64_t packet_len)
{
    int result = -1;
    if (p_cache)
    {
        p_cache->p_priv->packet_len = packet_len;
        p_cache->last_len           = 0;
        p_cache->buf                = cache_buf;

        if (packet_len <= MEMORY_CACHE_LEN && NULL == p_cache->buf) {
            leave_cache(p_cache);
            return result;
        }
        /* create cache file only for the first time */
        if (NULL == p_cache->p_priv->wfp 
            && p_cache->p_priv->packet_len > MEMORY_CACHE_LEN)
        {
            if (0 != create_cache_file(p_cache)) {
                return result;
            }
        }
        result = 0;
    }
    return result;
}

void leave_cache(cache_t *p_cache)
{
    if (p_cache) {
        p_cache->cache_len          = 0;
        p_cache->last_len           = 0;
        p_cache->buf                = NULL;
        memset(p_cache->path, 0, sizeof(p_cache->path));
        p_cache->index_name         = 0;
        p_cache->p_priv->packet_len = 0;
    }
}

/* return:  finish 0    unfinished 1    fail -1 */
int cache_recv_data(cache_t *p_cache, const char *p_data, uint32_t data_len)
{
    int result = -1;
    /* cache to memory */
    if (p_cache->p_priv->packet_len <= MEMORY_CACHE_LEN)
    {
		p_cache->p_priv->wlen = p_cache->p_priv->packet_len - p_cache->cache_len;
		if (p_cache->p_priv->wlen > data_len)
		{
			p_cache->p_priv->wlen = data_len;
        } else {
            p_cache->last_len = (uint32_t)p_cache->p_priv->wlen;
        }
        memcpy(p_cache->buf + p_cache->cache_len, p_data, (size_t)p_cache->p_priv->wlen);

        p_cache->cache_len += p_cache->p_priv->wlen;

        if  (p_cache->cache_len >= p_cache->p_priv->packet_len) 
        {
            result = 0;
            goto cache_end;
        }
        return 1;
    }
    /* cache to file */
    else
    {
        p_cache->p_priv->wlen = p_cache->p_priv->packet_len - p_cache->cache_len;
		if (p_cache->p_priv->wlen > data_len)
		{
			p_cache->p_priv->wlen = data_len;
        } else {
            p_cache->last_len = (uint32_t)p_cache->p_priv->wlen;
        }
        p_cache->cache_len += (uint64_t)fwrite(p_data, 1, p_cache->p_priv->wlen, 
                                               p_cache->p_priv->wfp);
        if (p_cache->cache_len >= p_cache->p_priv->packet_len) 
        {
            fclose(p_cache->p_priv->wfp);
            p_cache->p_priv->wfp = NULL;
            
            result = 0;
            goto cache_end;
        }
        return 1;
    }
cache_end:
    return result;
}

void destroy_cache(cache_t *p_cache)
{
    if (p_cache) {
        remove(p_cache->path);

        if (p_cache->p_priv) {
            free(p_cache->p_priv);
        }
        free(p_cache);
    }
}

