
#ifndef BORING_TOOLS_HH
#define BORING_TOOLS_HH

#include <stdint.h>
#include <stdbool.h>

/* if "order" is false, output with reverse order
 * else, output with memory order
 */
void show_fake_bits(const void *p_value, uint32_t value_len, bool order);


#endif /* BORING_TOOLS_HH */
