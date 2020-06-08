
#ifndef FORMATTED_TEXT_HH
#define FORMATTED_TEXT_HH

#include <stdbool.h>
#include <stdint.h>

/* 10M text file size limit */
#define MAXIMUM_FILE_SIZE   10485760

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t fomat_flags_t;
typedef struct format_officer format_officer_t;

#define FORMAT_UNDEFINED    0

/* Insert spaces between text */
#define FORMAT_ADD_SPACE    (1 << 1)
/* eat the space at the end of a line */
#define FORMAT_EAT_SPACE    (1 << 2)

#define FORMAT_FMT_CHECK    (1 << 29)
#define FORMAT_UNDISPLAY    (1 << 30)
#define FORMAT_TO_BACKUP    (1 << 31)

format_officer_t *format_officer_create();

bool formatting_check(format_officer_t *p_format,
                      const char *p_path, fomat_flags_t flags);

bool formatting_file(format_officer_t *p_format,
                     const char *p_path, const char *p_save_path,
                     fomat_flags_t flags);

void format_officer_destroy(format_officer_t *p_format);

#ifdef __cplusplus
}
#endif

#endif /* FORMATTED_TEXT_HH */
