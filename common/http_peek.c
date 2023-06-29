#include <string.h>
#include "http_peek.h"

// refer: https://github.com/h2o/picohttpparser
#if __GNUC__ >= 3
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

#define IS_PRINTABLE_ASCII(c) ((unsigned char)(c)-040u < 0137u)
#define CHAR_IS_NOT_NUMBER(c) (c < '0' || '9' < c)

#define CHECK_EOF() { \
    if (buf == buf_end) { \
        return result;\
    } \
}

#define EXPECT_CHAR(ch) \
    CHECK_EOF(); \
    if (*buf++ != ch) { \
        return result; \
    }

#define ADVANCE_TOKEN(tok, toklen) \
    do { \
        const char *tok_start = buf; \
        while (1) { \
            if (*buf == ' ') { \
                break; \
            } else if (unlikely(!IS_PRINTABLE_ASCII(*buf))) { \
                if ((unsigned char)*buf < '\040' || *buf == '\177') { \
                    return result; \
                } \
            } \
            ++buf; \
            CHECK_EOF(); \
        } \
        tok = tok_start; \
        toklen = (uint16_t)(buf - tok_start); \
    } while (0)

static const char *token_char_map =
"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
"\0\1\0\1\1\1\1\1\0\0\1\1\0\1\1\0\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0"
"\0\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\1\1"
"\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\1\0\1\0"
"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

static const char *
get_token_to_eol(const char *buf, const char *buf_end,
                 const char **token, uint16_t *token_len)
{
    const char *result = NULL;
    const char *token_start = buf;

    /* find non-printable char within the next 8 bytes */
    while (likely(buf_end - buf >= 8)) {
#define DOIT() \
        do {  \
            if (unlikely(!IS_PRINTABLE_ASCII(*buf))) \
            goto non_printable; \
            ++buf; \
        } while (0)
        DOIT();
        DOIT();
        DOIT();
        DOIT();
        DOIT();
        DOIT();
        DOIT();
        DOIT();
#undef DOIT
        continue;
non_printable:
        if ((likely((unsigned char)*buf < '\040') &&
             likely(*buf != '\011')) || unlikely(*buf == '\177'))
        {
            goto FOUND_CTL;
        }
        ++buf;
    }
    for (;; ++buf)
    {
        CHECK_EOF();
        if (unlikely(!IS_PRINTABLE_ASCII(*buf)))
        {
            if ((likely((unsigned char)*buf < '\040')
                 && likely(*buf != '\011')) || unlikely(*buf == '\177'))
            {
                goto FOUND_CTL;
            }
        }
    }
FOUND_CTL:
    if (likely(*buf == '\015')) {
        ++buf;
        EXPECT_CHAR('\012');
        *token_len = (uint16_t)(buf - 2 - token_start);
    } else if (*buf == '\012') {
        *token_len = (uint16_t)(buf - token_start);
        ++buf;
    } else {
        return result;
    }
    *token = token_start;

    return buf;
}

/* returned pointer is always within [buf, buf_end), or null */
static const char *
parse_token(const char *buf, const char *buf_end, const char **token,
            uint16_t *token_len, char next_char)
{
    const char *result = NULL;
    const char *buf_start = buf;
    while (1)
    {
        if (*buf == next_char) {
            break;
        } else if (!token_char_map[(unsigned char)*buf]) {
            return result;
        }
        ++buf;
        CHECK_EOF();
    }
    *token = buf_start;
    *token_len = (uint16_t)(buf - buf_start);
    return buf;
}

/* returned pointer is always within [buf, buf_end), or null */
static const char *
parse_http_version(const char *buf, const char *buf_end, int *version)
{
    const char *result = NULL;
    /* we want at least [HTTP/1.<two chars>] to try to parse */
    if (buf_end - buf < 9) {
        return result;
    }
    if (buf[0] != 'H' || buf[1] != 'T' || buf[2] != 'T' ||
        buf[3] != 'P' || buf[4] != '/' || buf[5] != '1' || buf[6] != '.')
    {
        return result;
    } else {
        buf += 7;
    }
    if (*buf < '0' || '9' < *buf)
    {
        buf++;
        return result;
    }
    *version = (*buf++ - '0');
    return buf;
}

static bool
parse_header(const char *buf, const char *buf_end,
             peek_header_t *p_head)
{
    bool result = false;
    CHECK_EOF();
    if (*buf == '\015') {
        ++buf;
        EXPECT_CHAR('\012');
        goto parse_end;
    } else if (*buf == '\012') {
        ++buf;
        goto parse_end;
    }
    if (!(*buf == ' ' || *buf == '\t'))
    {
        /* parsing name, but do not discard SP before colon */
        if ((buf = parse_token(buf, buf_end, &p_head->name,
                               &p_head->name_len, ':')) == NULL) {
            return result;
        }
        if (p_head->name_len == 0) {
            return result;
        }
        ++buf;
        for (;; ++buf) {
            CHECK_EOF();
            if (!(*buf == ' ' || *buf == '\t')) {
                break;
            }
        }
    } else {
        p_head->name = NULL;
        p_head->name_len = 0;
    }
    uint16_t value_len;
    if ((buf = get_token_to_eol(buf, buf_end,
                                &p_head->value, &value_len)) == NULL) {
        return result;
    }
    /* remove trailing SPs and HTABs */
    const char *value_end = p_head->value + value_len;
    for (; value_end != p_head->value; --value_end) {
        const char c = *(value_end - 1);
        if (!(c == ' ' || c == '\t')) {
            break;
        }
    }
    p_head->value_len = (uint16_t)(value_end - p_head->value);
    result = true;
parse_end:
    return result;
}

bool peek_http_request(const char *buf, uint16_t len, peek_header_t *p_head)
{
    bool result = false;
    const char *buf_end = buf + len;

    /* skip first empty line (some clients add CRLF after POST content) */
    CHECK_EOF();
    if (*buf == '\015') {
        ++buf;
        EXPECT_CHAR('\012');
    } else if (*buf == '\012') {
        ++buf;
    }
    /* parse request line */
    if ((buf = parse_token(buf, buf_end, &p_head->method,
                           &p_head->method_len, ' ')) == NULL) {
        return result;
    }
    do {
        ++buf;
        CHECK_EOF();
    } while (*buf == ' ');
    ADVANCE_TOKEN(p_head->path, p_head->path_len);
    do {
        ++buf;
        CHECK_EOF();
    } while (*buf == ' ');
    if (p_head->method_len == 0 || p_head->path_len == 0) {
        return result;
    }
    if ((buf = parse_http_version(buf, buf_end, &p_head->version)) == NULL) {
        return result;
    }
    if (*buf == '\015') {
        ++buf;
        EXPECT_CHAR('\012');
    } else if (*buf == '\012') {
        ++buf;
    } else {
        return result;
    }
    return parse_header(buf, buf_end, p_head);
}

bool peek_http_response(const char *buf, uint16_t len, peek_header_t *p_head)
{
    bool result = false;
    const char *buf_end = buf + len;
    /* parse "HTTP/1.x" */
    if ((buf = parse_http_version(buf, buf_end, &p_head->version)) == NULL) {
        return result;
    }
    /* skip space */
    if (*buf != ' ') {
        return result;
    }
    do {
        ++buf;
        CHECK_EOF();
    } while (*buf == ' ');
    /* parse status code, we want at least
     * [:digit:][:digit:][:digit:]<other char> to try to parse */
    if (buf_end - buf < 4 || CHAR_IS_NOT_NUMBER(buf[0]) ||
        CHAR_IS_NOT_NUMBER(buf[1]) || CHAR_IS_NOT_NUMBER(buf[2])) {
        return result;
    }
    else  // http status code is a 3-digit integer
    {
        p_head->res_code = 100 * (buf[0] - '0');
        p_head->res_code += 10 * (buf[1] - '0');
        p_head->res_code += (buf[2] - '0');
        buf += 3;
    }
    /* get message including preceding space */
    if ((buf = get_token_to_eol(buf, buf_end, &p_head->value,
                                &p_head->value_len)) == NULL) {
        return result;
    }
    if (p_head->msg_len == 0) {
        /* ok */
    } else if (*p_head->msg == ' ') {
        /* remove preceding space. */
        do {
            ++p_head->msg;
            --p_head->msg_len;
        } while (*p_head->msg == ' ');
    } else {
        /* garbage found after status code */
        return result;
    }
    return parse_header(buf, buf_end, p_head);
}

