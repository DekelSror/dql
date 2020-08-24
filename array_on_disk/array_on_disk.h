#ifndef dql_array_on_disk_h
#define dql_array_on_disk_h

#include <stddef.h>

typedef void* array_on_disk_t;


typedef struct
{
    size_t _length;
    unsigned _element_size;
    char _array[];
} aod_filedata_t;


typedef struct 
{
    array_on_disk_t(*create)(size_t length, unsigned element_size);
    array_on_disk_t(*from_file)(int);
    size_t(*len)(const array_on_disk_t);
    void*(*at)(array_on_disk_t, size_t);

    // void(*free)(array_on_disk_t);
    
} array_on_disk_api_t;


extern const array_on_disk_api_t Array_on_disk;



#endif // dql_array_on_disk_h
