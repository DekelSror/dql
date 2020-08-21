#ifndef dql_strings_with_rc_string_h
#define dql_strings_with_rc_string_h

#include "string_class.h"


typedef void* strings_t;

typedef struct 
{
    strings_t(*create)(void);
    void(*free)(strings_t);
    //
    string_t*(*get)(strings_t,  const char*);
    void(*dispose)(strings_t, string_t*);

} strings_api_t; 

extern const strings_api_t Strings;


#endif // dql_strings_with_rc_string_h