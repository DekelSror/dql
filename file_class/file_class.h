#ifndef dql_file_class_h
#define dql_file_class_h


#include <stddef.h>

typedef void* file_t;

typedef struct 
{
    file_t(*create)(const char*);
    // int(*write)(void*, const char*, size_t);
    file_t(*open_ro)(const char*);
    char*(*read)(void*, size_t);
} file_api_t;

extern const file_api_t File;



#endif // dql_file_class_h
