
#include <stdlib.h>

#define declare_array_st(type) \
typedef struct \
{\
    unsigned _size;\
    type _at[]; \
} type##_arr;

#define declare_array_ctor(type) \
type##_arr* type##ArrayCreate(unsigned size) \
{ \
    type##_arr* arr = (type##_arr*)malloc(sizeof(*arr) + (sizeof(type) * size)); \
    if (arr != NULL) arr->_size = size; \
    return arr; \
}


#define declare_array(type) \
declare_array_st(type) \
declare_array_ctor(type) \


