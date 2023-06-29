#ifndef HTTP_PEEK_HH
#define HTTP_PEEK_HH

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// refer: https://github.com/h2o/picohttpparser
typedef struct {
    /* common */
    const char *name;
    uint16_t name_len;
    const char *value; // in the request, it is a URL
    uint16_t value_len;

    int version;
    /* request */
    const char *method;
    uint16_t method_len;
    const char *path;
    uint16_t path_len;
    /* response */
    int res_code;
    const char *msg;
    uint16_t msg_len;

} peek_header_t;

bool peek_http_request(const char *buf, uint16_t len, peek_header_t *p_head);

bool peek_http_response(const char *buf, uint16_t len, peek_header_t *p_head);

#ifdef __cplusplus
}
#endif

#endif // HTTP_PEEK_HH
