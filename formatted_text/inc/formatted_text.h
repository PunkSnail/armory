
#ifndef FORMATTED_TEXT_HH
#define FORMATTED_TEXT_HH

/* 10M text file size limit */
#define MAXIMUM_FILE_SIZE   10485760

/* formatted text file
 * return:   succ 0     fail -1 */
int formatted_text(const char *text_path);


#endif /* FORMATTED_TEXT_HH */
