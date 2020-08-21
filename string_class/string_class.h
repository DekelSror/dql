#ifndef dql_string_class_h
#define dql_string_class_h

#include <stddef.h>

typedef struct _string_ string_t;

typedef struct
{
    // basic ctor
    string_t*(*create)(const char* data);

    // ctor with fixed length
    string_t*(*create_fixed)(const char *data, size_t len);

    // ctor until occurence of end_str, if exists
    string_t*(*create_until)(const char *data, const char* end_str);
    
    // dtor
    void (*free)(string_t* str);

    // chars
    const char*(*chars)(const string_t* str);
    
    // add to_add to str
    string_t*(*add_str)(string_t* str, string_t* to_add);

    // add to_add to str
    string_t*(*add_char)(string_t* str, const char* to_add);

    int(*cmp)(const string_t* a, const string_t* b);
    
    // ==
    int (*eq_ss)(const string_t* a, const string_t* b);

    // ==
    int (*eq_sc)(const string_t* a, const char* b);

    // ==
    int (*eq_cs)(const char* a, const string_t* b);

    // does a contain b ?
    int (*has)(const string_t* a, const string_t* b);

    // cctor
    string_t*(*copy)(const string_t* str);

    // lentgh of str
    size_t(*len)(const string_t* str);

    // does not modify str
    string_t*(*remove_whitespace)(const char* str);
} string_api_t;

extern const string_api_t String;

#endif // dql_string_class_h