#ifndef UPLOAD_REPORT_HHH
#define UPLOAD_REPORT_HHH
#include <stdint.h>
#include <stdbool.h>

#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_URL_LEN 1024

typedef enum 
{
    UPLOAD_TYPE_BUF = 1,
    UPLOAD_TYPE_FILE,

}upload_type_t;

typedef struct
{
    upload_type_t type;
    const char *url;        // Cannot be NULL
    uint32_t timeout;       // Unit second
    const char *buf;
    uint64_t buf_size;

    /*for different upload type*/
    const char *name;

}upload_info_t;

typedef struct upload_handler upload_handler_t;

typedef void (*state_cb_t)(bool succ, int32_t http_code, 
                           IN upload_info_t *p_upload, IN void *p_param);

/* active reading of external data */
typedef void (*active_cb_t)(IN void *p_active_param, 
                            OUT upload_info_t **pp_info, OUT void **pp_param);

/*return:   succ p_upload     error NULL */
upload_handler_t *init_upload_handler();

void set_upload_cb(upload_handler_t *p_upload, state_cb_t state_cb, 
                   active_cb_t active_cb, void *p_active_param);

/* If success function is blocked */
void run_upload(upload_handler_t *p_upload);

void stop_upload(upload_handler_t *p_upload);

/* p_param for state_cb_t
 * return:   succ 0     error -1    fail http code */
int common_upload(upload_handler_t *p_upload, upload_info_t *p_info, void *p_param);

void release_upload_handler(upload_handler_t *p_upload);

#ifdef __cplusplus
}
#endif

#endif
