
#include "iplib_reader.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// refer: https://github.com/lionsoul2014/ip2region

#define INDEX_BLOCK_LENGTH 12

/* get the start of buffer from the offset */
static inline uint32_t
get_buffer_offset(const char *buffer, uint32_t offset)
{
    return (((uint32_t)(buffer[offset] << 0) & 0x000000FF) |
            ((uint32_t)(buffer[offset+1] <<  8) & 0x0000FF00) |
            ((uint32_t)(buffer[offset+2] << 16) & 0x00FF0000) |
            ((uint32_t)(buffer[offset+3] << 24) & 0xFF000000));
}

static bool load_db_to_mem(iplib_reader_t *p_reader, FILE *db_fd)
{
    if (p_reader->db_mem != NULL) {
        return true;
    }
    //get the size of the file
    fseek(db_fd, 0, SEEK_END);
    uint32_t filesize = (uint32_t)ftell(db_fd);
    fseek(db_fd, 0, SEEK_SET);

    p_reader->db_mem = (char*)malloc(filesize);
    if (p_reader->db_mem == NULL ) {
        return false;
    }
    if (fread(p_reader->db_mem, filesize, 1, db_fd) != 1 ) {
        return false;
    }
    char *buffer = p_reader->db_mem;
    p_reader->first_index = get_buffer_offset(buffer, 0);
    p_reader->total_blocks =
        (get_buffer_offset(buffer, 4) - p_reader->first_index)
        / INDEX_BLOCK_LENGTH + 1;

    const char *end_str = p_reader->db_mem + (filesize - 8);

    if (strstr(end_str, "PUNK")) { // the punk IP lib ends with this tag
        p_reader->is_punk_lib = true;
    } else {
        p_reader->is_punk_lib = false;
    }
    return true;
}

/* create a new iplib_reader_t instance */
iplib_reader_t *iplib_reader_create(const char *db_file_path)
{
    iplib_reader_t *p_reader = (iplib_reader_t*)
        calloc(1, sizeof(iplib_reader_t));

    // open the db file
    FILE *db_fd = fopen(db_file_path, "rb");
    if (db_fd == NULL) {
        fprintf(stderr, "failed to open the db file %s\n",
                db_file_path);
        goto create_error_end;
    }
    if (!load_db_to_mem(p_reader, db_fd)) {
        fprintf(stderr, "failed to load the db file to memory\n");
        goto create_error_end;
    }
    fclose(db_fd);
    return p_reader;

create_error_end:
    if (db_fd) {
        fclose(db_fd);
    }
    iplib_reader_destroy(p_reader);
    return NULL;
}

static inline char *
get_data_info(iplib_reader_t *p_reader, uint32_t ip, uint16_t *p_len)
{
    char *p_data = NULL;
    uint32_t offset = 0, blocks = p_reader->total_blocks;

    for (uint32_t curr, i = 0; i <= blocks; )
    {
        curr = (i + blocks) >> 1;

        p_data = p_reader->db_mem +
            p_reader->first_index + curr * INDEX_BLOCK_LENGTH;

        if (ip < get_buffer_offset(p_data, 0)) { // less then start
            blocks = curr - 1;
        } else {
            if (ip > get_buffer_offset(p_data, 4)) { // greater then end
                i = curr + 1;
            } else {
                offset = get_buffer_offset(p_data, 8);
                break;
            }
        }
    }
    if (offset == 0)  {
        return NULL;
    }
    p_data = p_reader->db_mem + (offset & 0x00FFFFFF);
    *p_len = (uint8_t)((offset >> 24) & 0xFF);

    if (false == p_reader->is_punk_lib && *p_len > 4)
    {
        *p_len = (uint16_t)(*p_len - 4);
        p_data += 4; /* skip the int32_t id field */
    }
    if (p_reader->is_punk_lib && *p_len == 0xFF)
    {
        memcpy(p_len, p_data, sizeof(uint16_t));
        *p_len = (uint16_t)(*p_len - 2);
        p_data += 2; /* skip length field */
    }
    return p_data;
}

/* return: C string describe
 * refer:
 * https://github.com/lionsoul2014/ip2region/tree/master/binding/c */
const char *iplib_reader_search(iplib_reader_t *p_reader, uint32_t ip,
                                char *output, uint32_t output_len)
{
    const char *result = "empty";
    uint16_t data_len = 0;
    char *p_data = get_data_info(p_reader, ip, &data_len);

    if (NULL == p_data)
    {
        return result;
    }
    if (output && data_len < output_len)
    {
        memcpy(output, p_data, data_len);
        output[data_len] = '\0';
        result = output;
    }
    else if (data_len < MAX_DESCRIBE_LENGTH)
    {   // output is NULL, the internal describe_cache will be used
        strncpy(p_reader->describe_cache, p_data, data_len);
        p_reader->describe_cache[data_len] = '\0';
        result = p_reader->describe_cache;
    }
    return result;
}

const char *iplib_reader_search_s(iplib_reader_t *p_reader, uint32_t ip)
{
     return iplib_reader_search(p_reader, ip, NULL, 0);
}

/* destroy the specifield iplib_reader object */
void iplib_reader_destroy(iplib_reader_t *p_reader)
{
    if (p_reader == NULL) {
        return;
    }
    // free the db binary string
    if (p_reader->db_mem != NULL ) {
        free(p_reader->db_mem);
        p_reader->db_mem = NULL;
    }
    free(p_reader);
}

