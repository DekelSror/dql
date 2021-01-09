#include <string.h>
#include <stdlib.h>

#include "string_class.h"

#define string_thisify _string_t* this = _this;


// object
typedef struct 
{
    size_t length;
    char str[];
} _string_t;

// utils decl
static string_t StrAddBase(string_t base, const char* a, const char* b, size_t len_a, size_t len_b);


// api imp decl
static string_t StrCreate(const char* data) ;
static string_t StrCreateFixed(const char* data, size_t len);
static string_t StrCreateUntil(const char* data, const char* end_str);
//
static void StrFree(string_t str);
//
static const char* StrChars(const string_t str) { return ((_string_t*)str)->str; }
//
static string_t StrAddStr(string_t str, string_t to_add);
static string_t StrAddChar(string_t str, const char* to_add);
//
static int StrCmp(const string_t a, const string_t b);
//
static int StrIsEqual_SS(const string_t a, const string_t b);
static int StrCmp_SC(const string_t a, const char* b);
static int StrCmp_CS(const char* a, const string_t b);
//
static int StrHas(const string_t a, const string_t b);
static string_t StrCopy(const string_t str);
static size_t StrLen(const string_t str);
//
static string_t Replace(string_t _src, string_t _old, string_t new);


// class object definition
const string_api_t String = 
{
    StrCreate,
    StrCreateFixed,
    StrCreateUntil,
    //
    StrFree,
    //
    StrChars,
    //
    StrAddStr,
    StrAddChar,
    //
    StrCmp,
    //
    StrIsEqual_SS,
    StrCmp_SC,
    StrCmp_CS,
    //
    StrHas,
    StrCopy,
    StrLen,
    //
    Replace
};

int StrCmp(const string_t _a, const string_t _b)
{
    const _string_t* a = _a; 
    const _string_t* b = _b;
    return strcmp(a->str, b->str);
}

// api implementation

static string_t StrCreate(const char* data) 
{
    size_t len = strlen(data);

    return StrCreateFixed(data, len);    
}

static string_t StrCreateFixed(const char* data, size_t len)
{
    _string_t* str = malloc(sizeof(*str) + len + 1);

    if (NULL == str) return NULL;

    str->length = len;
    memmove(str->str, data, len);
    str->str[len] = 0;

    return str;
}

static string_t StrCreateUntil(const char* data, const char* end_str)
{
    char* parting_point = strstr(data, end_str);

    if (parting_point) return StrCreateFixed(data, parting_point - data);

    return NULL;
}

//

static void StrFree(string_t str)
{
    free(str);
    str = NULL;
}

//

static string_t StrAddStr(string_t _str, string_t _to_add)
{
    _string_t* str = _str;
    _string_t* to_add = _to_add;
    return StrAddBase(str, str->str, to_add->str, str->length, to_add->length);
}

static string_t StrAddChar(string_t _str, const char* to_add)
{
    _string_t* str = _str;
    return StrAddBase(str, str->str, to_add, str->length, strlen(to_add));
}

//

static int StrIsEqual_SS(const string_t _a, const string_t _b)
{
    const _string_t* a = _a; 
    const _string_t* b = _b;
    return !(0 == strcmp(a->str, b->str));
}

static int StrCmp_SC(const string_t _a, const char* b)
{
    const _string_t* a = _a; 
    return !(0 == strcmp(a->str, b));
}

static int StrCmp_CS(const char* a, const string_t _b)
{
    const _string_t* b = _b;
    return !(0 == strcmp(a, b->str));
}

//

static int StrHas(const string_t _a, const string_t _b)
{
    const _string_t* a = _a; 
    const _string_t* b = _b;
    return NULL != strstr(a->str, b->str);
}

static string_t StrCopy(const string_t _str)
{
    const _string_t* str = _str;

    return StrCreate(str->str);
}

static size_t StrLen(const string_t _str)
{
    const _string_t* str = _str;
    return str->length + 1;
}


// utils implementation
static string_t StrAddBase(string_t base, const char* a, const char* b, size_t len_a, size_t len_b)
{
    (void)base;
    _string_t* str = malloc(sizeof(*str) + len_a + len_b + 1);

    memmove(str->str, a, len_a);
    memmove(str->str + len_a, b, len_b);

    str->str[len_a + len_b] = 0;

    str->length = len_a + len_b;

    return str;
}


static string_t Replace(string_t _src, string_t _old, string_t new)
{
    _string_t* src = _src, *old = _old;
    char *src_str = src->str, *old_str = old->str;
    size_t src_len = src->length, old_len = old->length;

    _string_t* res = String.create("");


    while (src_str < (src->str + src_len))
    {
        char* occurence = strstr(src_str, old_str);

        size_t part_len = occurence - src_str;

        _string_t* part = String.create_fixed(src_str, part_len);

        String.add_str(res, part);
        String.add_str(res, new);

        src_str += part_len;

        String.free(part);
    }


    return res;
}
