
#ifndef DEBUG_PRINT_HHH
#define DEBUG_PRINT_HHH

#include <stdio.h>

#ifdef _DEBUG

#define DEBUG_PRINT(fmt, args...)\
    do{\
        printf("%s:%d "fmt, __FILE__, __LINE__, ##args);\
    } while(0)

#define debug_print(...) do{ printf(__VA_ARGS__); } while(0)

#else /* _DEBUG */

#define DEBUG_PRINT(...) do{ } while(0)

#define debug_print(...) do{ } while(0)

#endif /* _DEBUG */


#endif /* DEBUG_PRINT_HHH */
