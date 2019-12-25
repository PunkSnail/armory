
#include <stdio.h>
#include "boring_tools.h"

#define line() printf("\n------------------------------\n");

void test_show_fake_bits(void)
{
    uint32_t value = 3;
    printf("uint32_t \"%u\" reverse order:\n", value);
    show_fake_bits(&value, sizeof(value), false);
    
    char buf[] = "abc";

    printf("string \"%s\" memory order:\n", buf);
    show_fake_bits(buf, sizeof(buf), true);
}

void test_swp_ptr(void)
{
    const char *a = "this is a string";
    const char *b = "this is b string";

    printf("\na: %s\tb: %s\n", a, b);

    printf("swap:");

    SWAP_PTR(a, b);

    printf("\na: %s\tb: %s\n", a, b);
}

int main(int argc, char *argv[])
{
    test_swp_ptr();
    line();
    test_show_fake_bits();

    return 0;
}

