
#ifndef BORING_TOOLS_HH
#define BORING_TOOLS_HH

#include <stdint.h>
#include <stdbool.h>

#define SWAP_PTR(p1, p2) \
    p1 = (void*)((uint64_t)p1 ^ (uint64_t)p2); \
    p2 = (void*)((uint64_t)p1 ^ (uint64_t)p2); \
    p1 = (void*)((uint64_t)p1 ^ (uint64_t)p2);

/* if "order" is false, output in reverse order
 * otherwise, output in memory order
 */
void show_fake_bits(const void *p_value, uint32_t value_len, bool order);


#endif /* BORING_TOOLS_HH */
