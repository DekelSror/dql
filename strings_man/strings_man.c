#include <stdlib.h>

#include "strings_man.h"
#include "hash.h"

#define initial_num_of_strings (64)


typedef struct 
{
    string_t* _string;
    long _rc;
} rc_string_t;


static strings_t Create(void)
{
    return Hash.create(initial_num_of_strings);    
}

static void Free(strings_t strings)
{
    Hash.free(strings);
}

static string_t* GetString(strings_t strings, const char* str)
{    
    rc_string_t* rcs = Hash.get(strings, str);

    if (NULL != rcs) // exist. increment rc
    {
        ++rcs->_rc;

        return rcs->_string;
    }

    rcs = malloc(sizeof(*rcs));
    rcs->_string = String.create(str);
    
    int set_rc = Hash.set(strings, String.chars(rcs->_string), (void*)1);
    
    if (set_rc) return NULL;
}

static void FreeString(strings_t strings, string_t* str)
{
    rc_string_t* rcs = Hash.get(strings, String.chars(str));

    if (NULL != rcs)
    {
        --rcs->_rc;
        if (rcs->_rc <= 0)
        {
            Hash.remove(strings, str);
            String.free(rcs->_string);
            free(rcs);
        }
    }
}

const strings_api_t Strings = {Create, Free, GetString, FreeString};





