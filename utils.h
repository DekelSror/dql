#ifndef dql_general_utils_h
#define dql_general_utils_h

#include <stdlib.h>
#include <stdarg.h>

#include "types.h"
#include "string_class.h"


#define IS_WS(c) (' ' == c || '\t' == c || '\n' == c)

const char* SkipWhiteSpace(const char* str)
{
    while (IS_WS(*str)) ++str;

    return str;
}

string_t* GetWord(const char* str) {
    const char* start = SkipWhiteSpace(str);    

    const char* end = start;
    while (!IS_WS(*end) && *end != '|' && *end) 
    {
        ++end;
    }

    return String.create_fixed(start, end - start);
}


#endif // dql_general_utils_h
