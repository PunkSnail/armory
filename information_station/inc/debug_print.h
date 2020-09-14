#ifndef DEBUG_PRINT_HH
#define DEBUG_PRINT_HH

#include <stdio.h>

#ifdef _DEBUG

#define DEBUG_PRINT(fmt, args...) \
{ \
    printf("%s:%d " fmt, __FILE__, __LINE__, ##args); \
}

#define debug_print(...) \
{ \
    printf(__VA_ARGS__); \
}

#else /* _DEBUG */

#define DEBUG_PRINT(...) {  }

#define debug_print(...) {  }

#endif /* _DEBUG */


#endif /* DEBUG_PRINT_HH */
