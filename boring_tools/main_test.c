
#include <stdio.h>
#include "boring_tools.h"

void test_show_fake_bits(void)
{
    uint32_t value = 3;
    show_fake_bits(&value, sizeof(value), false);
    
    printf("----------------------\n");
    char buf[10] = "abc";

    show_fake_bits(buf, 10, true);
}

int main(int argc, char *argv[])
{
    test_show_fake_bits();

    return 0;
}

