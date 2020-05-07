
#ifndef FORMATTED_TEXT_HH
#define FORMATTED_TEXT_HH

#include <stdbool.h>

/* 10M text file size limit */
#define MAXIMUM_FILE_SIZE   10485760

#ifdef __cplusplus
extern "C" {
#endif

/* formatted text file */
bool formatted_text(const char *p_path);

/* eat the space at the end of a line */
bool eat_trailing_space(const char *p_path);

#ifdef __cplusplus
}
#endif

#endif /* FORMATTED_TEXT_HH */
