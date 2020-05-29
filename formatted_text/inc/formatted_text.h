
#ifndef FORMATTED_TEXT_HH
#define FORMATTED_TEXT_HH

#include <stdbool.h>

/* 10M text file size limit */
#define MAXIMUM_FILE_SIZE   10485760

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    FORMAT_ADD_SPACE, /* formatted text file */
    FORMAT_EAT_SPACE, /* eat the space at the end of a line */

    FORMAT_UNDEFINED,

}fomat_type_t;

bool formatting_file(const char *p_path, const char *p_save_path,
                     fomat_type_t type, bool backup);

#ifdef __cplusplus
}
#endif

#endif /* FORMATTED_TEXT_HH */
