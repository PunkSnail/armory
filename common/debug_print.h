
#ifndef DEBUG_PRINT_HHH
#define DEBUG_PRINT_HHH

#include <stdio.h>

#ifdef _DEBUG

#define DEBUG_PRINT(...)\
    do{\
        fprintf(stderr, "%s:%d ", __FILE__, __LINE__);\
        fprintf(stderr, __VA_ARGS__);\
    }while(0)

#define debug_print(...) do{ fprintf(stderr, __VA_ARGS__); }while(0)

#else /* _DEBUG */

#define DEBUG_PRINT(...) do{ }while(0)

#define debug_print(...) do{ }while(0)

#endif /* _DEBUG */


#endif /* DEBUG_PRINT_HHH */
