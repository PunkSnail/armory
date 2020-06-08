
#ifndef UTILS_LIBRARY_HH
#define UTILS_LIBRARY_HH

#if defined(__unix__) || defined(__linux__)

#define LIB_PUBLIC __attribute__ ((visibility ("default")))
#define LIB_LOCAL __attribute__ ((visibility ("hidden")))

#elif defined(WIN32)

#define LIB_PUBLIC __declspec(dllexport)
#define LIB_LOCAL

#endif

void *open_library(const char *p_lib_path, ...);

void *get_symbol_address(void *p_handle, const char *p_symbol_name);

void close_library(void *p_handle);

#endif /* UTILS_LIBRARY_HH */

