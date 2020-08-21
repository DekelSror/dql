#include <string.h>
#include <stdlib.h>

#include "string_class.h"


// object
struct _string_
{
    size_t length;
    char str[];
};

// utils decl
static string_t* StrAddBase(string_t* base, const char* a, const char* b, size_t len_a, size_t len_b);


// api imp decl
static string_t* StrCreate(const char* data) ;
static string_t* StrCreateFixed(const char* data, size_t len);
static string_t* StrCreateUntil(const char* data, const char* end_str);
//
static void StrFree(string_t* str);
//
static const char* StrChars(const string_t* str) { return str->str; }
//
static string_t* StrAddStr(string_t* str, string_t* to_add);
static string_t* StrAddChar(string_t* str, const char* to_add);
//
static int StrCmp(const string_t* a, const string_t* b);
//
static int StrIsEqual_SS(const string_t* a, const string_t* b);
static int StrCmp_SC(const string_t* a, const char* b);
static int StrCmp_CS(const char* a, const string_t* b);
//
static int StrHas(const string_t* a, const string_t* b);
static string_t* StrCopy(const string_t* str);
static size_t StrLen(const string_t* str);
//
static string_t* StrRemoveWS(const char* str);


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
    StrRemoveWS
};

int StrCmp(const string_t* a, const string_t* b)
{
    return strcmp(a->str, b->str);
}

// api implementation

static string_t* StrCreate(const char* data) 
{
    size_t len = strlen(data);

    return StrCreateFixed(data, len);    
}

static string_t* StrCreateFixed(const char* data, size_t len)
{
    string_t* str = malloc(sizeof(*str) + len + 1);

    if (NULL == str) return NULL;

    str->length = len;
    memmove(str->str, data, len);
    str->str[len] = 0;

    return str;
}

static string_t* StrCreateUntil(const char* data, const char* end_str)
{
    char* parting_point = strstr(data, end_str);

    if (parting_point) return StrCreateFixed(data, parting_point - data);

    return NULL;
}

//

static void StrFree(string_t* str)
{
    free(str);
    str = NULL;
}

//

static string_t* StrAddStr(string_t* str, string_t* to_add)
{
    return StrAddBase(str, str->str, to_add->str, str->length, to_add->length);
}

static string_t* StrAddChar(string_t* str, const char* to_add)
{
    return StrAddBase(str, str->str, to_add, str->length, strlen(to_add));
}

//

static int StrIsEqual_SS(const string_t* a, const string_t* b)
{
    return !(0 == strcmp(a->str, b->str));
}

static int StrCmp_SC(const string_t* a, const char* b)
{
    return !(0 == strcmp(a->str, b));
}

static int StrCmp_CS(const char* a, const string_t* b)
{
    return !(0 == strcmp(a, b->str));
}

//

static int StrHas(const string_t* a, const string_t* b)
{
    return NULL != strstr(a->str, b->str);
}

static string_t* StrCopy(const string_t* str)
{
    return StrCreate(str->str);
}

static size_t StrLen(const string_t* str)
{
    return str->length;
}


// utils implementation
static string_t* StrAddBase(string_t* base, const char* a, const char* b, size_t len_a, size_t len_b)
{
    (void)base;
    string_t* str = malloc(sizeof(*str) + len_a + len_b + 1);

    memmove(str->str, a, len_a);
    memmove(str->str, b, len_b);

    str->str[len_a + len_b] = 0;

    str->length = len_a + len_b;

    return str;
}

static string_t* StrRemoveWS(const char* str)
{
    return StrCreate(str);    
}
