
#include <stdio.h>
#include "boring_tools.h"

void test_show_fake_bits(void)
{
    uint32_t value = 3;
    printf("uint32_t \"%u\" reverse order:\n", value);
    show_fake_bits(&value, sizeof(value), false);
    
    char buf[10] = "abc";

    printf("string \"%s\" memory order:\n", buf);
    show_fake_bits(buf, sizeof(buf), true);
}

int main(int argc, char *argv[])
{
    test_show_fake_bits();

    return 0;
}

