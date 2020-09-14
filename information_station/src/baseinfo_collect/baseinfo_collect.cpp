
#include <iostream>
#include <fstream>      /* filebuf ... */
#include <string>
#include <vector>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>    /* pthread_mutex_t */

#include "debug_print.h"
#include "baseinfo_collect.h"

using namespace std;


typedef struct priv_item priv_item_t;

typedef bool (*collect_update_cb_t)(priv_item_t *p_item);

struct priv_item
{
    priv_item(): type(BASEINFO_TYPE_INVALID),
        is_dynamic(false), cb(NULL) {  }

    /* name of item instances */
    string name;
    /* It can store not only C-strings but also binary data */
    string data;

    baseinfo_type_t type;

    bool is_dynamic;
    collect_update_cb_t cb;
};

struct collect_handle
{
    collect_handle(): item_arr(NULL), item_count(0)
    {
        pthread_mutex_init(&this->lock, NULL);
    }
    ~collect_handle();

    collect_item_t *item_arr; /* Use calloc to allocate memory */
    uint32_t item_count;

    vector <priv_item_t> priv_item_vec;

    pthread_mutex_t lock;
};

collect_handle::~collect_handle()
{
    if (NULL != this->item_arr)
    {
        free(this->item_arr);
        this->item_arr = NULL;
    }
    pthread_mutex_destroy(&this->lock);
}

collect_handle_t *create_collect_handle(void)
{
    collect_handle_t *p_collect = new collect_handle_t;

    return p_collect;
}

static void collect_result_format(string & str)
{
    for (int i = 0; i < (int)str.size(); i++)
    {
         char c = str[i];

         if ('\t' == c || ' ' == c || ':' == c || '"' == c)
         {
             continue; /* Skip the damn prefixes */
         }
         str = str.c_str() + i;
         break;
    }
    for (int i = (int)str.size() - 1; i >= 0; i--)
    {
        if ('"' != str[i] && ' ' != str[i])
        {
            break;
        }
        str.pop_back();  /* Delete the damn suffixes */
    }
}

static bool
parse_info_by_file(const char *file, const char *key, string & data)
{
    bool result = false;

    filebuf fb;
    string line;

    if (fb.open(file, ios::in) == NULL)
    {
        DEBUG_PRINT("open error: %s\n", strerror(errno));
        return result;
    }
    istream file_stream(&fb);

    while (getline(file_stream, line, '\n'))
    {
        if (line.find(key) != string::npos)
        {
            data = line.c_str() + strlen(key);

            collect_result_format(data);
            result = true;
            break;
        }
    }
    fb.close();

    return result;
}

static bool collect_system_info(priv_item_t *p_item)
{
    const char *key = "PRETTY_NAME=";

    /* TODO: 外部定义词条名称 */
    p_item->name = "OS";
    p_item->type = BASEINFO_TYPE_CSTRING;

    return parse_info_by_file("/etc/os-release", key, p_item->data);
}

static bool collect_cpu_info(priv_item_t *p_item)
{
    const char *key = "model name";

    p_item->name = "CPU";
    p_item->type = BASEINFO_TYPE_CSTRING;

    return parse_info_by_file("/proc/cpuinfo", key, p_item->data);
}

static bool collect_static_infos(collect_handle_t *p_collect)
{
    bool result = false;

    priv_item_t item;

    if (true == collect_system_info(&item))
    {
        p_collect->priv_item_vec.push_back(item);
    }

    if (true == collect_cpu_info(&item))
    {
        p_collect->priv_item_vec.push_back(item);
    }
    if (0 != p_collect->priv_item_vec.size())
    {
        result = true;
    }
    return result;
}

static bool collect_mem_info(priv_item_t *p_item)
{
    bool result = false;

    const char *file = "/proc/meminfo";

    p_item->name = "Memory";
    p_item->type = BASEINFO_TYPE_CSTRING;
    p_item->cb = collect_mem_info;

    string mem_total;
    string mem_free;

    if (parse_info_by_file(file, "MemTotal", mem_total))
    {
        result = parse_info_by_file(file, "MemFree", mem_free);
    }
    if (result)
    {
        double total_n = (double)atol(mem_total.c_str());
        double free_n = (double)atol(mem_free.c_str());

        char temp[128] = { 0 };

        snprintf(temp, sizeof(temp), "%.0lfM / %.0lfM (%.1lf%% used)",
                 (total_n - free_n) / 1024, total_n / 1024,
                 ((total_n - free_n)) / total_n * 100);

        p_item->data = temp;
    }
    return result;
}

static bool collect_dynamic_infos(collect_handle_t *p_collect)
{
    bool result = false;

    size_t old_count = p_collect->priv_item_vec.size();

    priv_item_t item;
    item.is_dynamic = true; /* we are dynamic */

    if (true == collect_mem_info(&item))
    {
        p_collect->priv_item_vec.push_back(item);
    }

    if (old_count < p_collect->priv_item_vec.size())
    {
        result = true;
    }
    return result;
}

static bool collect_copy_item_arr(collect_handle_t *p_clt)
{
    bool result = false;
    pthread_mutex_lock(&p_clt->lock);

    if (NULL == p_clt->item_arr)
    {
        /* for the first time */
        p_clt->item_count = (uint32_t)p_clt->priv_item_vec.size();

        if (p_clt->item_count &&
            !(p_clt->item_arr = (collect_item_t*)
              calloc(p_clt->item_count, sizeof(collect_item_t))))
        {
            p_clt->item_count = 0;
            debug_print("%s:%s\n", __func__, strerror(errno));
        }
    }
    for (uint32_t i = 0; i < p_clt->item_count; i++)
    {
        collect_item_t *p_item = &p_clt->item_arr[i];
        priv_item_t *p_priv = &p_clt->priv_item_vec[i];

        p_item->type = p_priv->type;
        p_item->name = p_priv->name.c_str();

        p_item->p_data = (const uint8_t*)p_priv->data.c_str();
        p_item->data_len = (uint32_t)p_priv->data.size();
    }
    if (p_clt->item_count)
    {
        result = true;
    }
    pthread_mutex_unlock(&p_clt->lock);

    return result;
}

bool collect_infos(collect_handle_t *p_collect,
                   collect_item_t **p_item_arr, uint32_t *p_count)
{
    bool result = false;

    if (NULL == p_collect || NULL == p_item_arr || NULL == p_count)
    {
        DEBUG_PRINT("Invalid input parameters\n");
        return result;
    }
    if (false == collect_static_infos(p_collect))
    {
        debug_print("collect_static_infos error\n");
    }
    if (false == collect_dynamic_infos(p_collect))
    {
        debug_print("collect_dynamic_infos error\n");
    }
    result = collect_copy_item_arr(p_collect);

    *p_item_arr = p_collect->item_arr;

    *p_count = p_collect->item_count;

    return result;
}

bool update_collect_infos(collect_handle_t *p_collect)
{
    if (NULL == p_collect)
    {
         return false;
    }
    pthread_mutex_lock(&p_collect->lock);

    for (int i = 0; i < (int)p_collect->priv_item_vec.size(); i++)
    {
        priv_item_t *p_item = &p_collect->priv_item_vec[i];

        if (p_item->is_dynamic)
        {
            p_item->cb((priv_item_t*)p_item);
        }
    }
    pthread_mutex_unlock(&p_collect->lock);

    return collect_copy_item_arr(p_collect);
}

void destroy_collect_handle(collect_handle_t *p_collect)
{
     if (NULL != p_collect)
     {
         delete p_collect;
         p_collect = NULL;
     }
}

