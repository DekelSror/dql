#include <stdlib.h>

#include "strings_man.h"
#include "hash.h"

#define initial_num_of_strings (64)


typedef struct 
{
    string_t _string;
    long _rc;
} rc_string_t;


static strings_t Create(void)
{
    return Hash.create(initial_num_of_strings);
}

static void FreeInnerString(void* _this, void* arg)
{
    (void)arg;
    rc_string_t* this = _this;
    String.free(((rc_string_t*)this)->_string);
    free(this);
}

static void Free(strings_t strings)
{
    Hash.for_each(strings, FreeInnerString, NULL);
    Hash.free(strings);
}

static string_t GetString(strings_t strings, const char* _str)
{
    string_t str = String.create(_str);
    rc_string_t* rcs = Hash.get(strings, str);

    if (NULL != rcs) // exists. increment rc
    {
        ++rcs->_rc;

        String.free(str);

        return rcs->_string;
    }

    rcs = malloc(sizeof(*rcs));
    rcs->_string = str;
    rcs->_rc = 1;
    
    int set_rc = Hash.set(strings, rcs->_string, rcs);
    
    if (set_rc) return NULL; // free stuff?

    return str;
}

static void FreeString(strings_t strings, string_t str)
{
    rc_string_t* rcs = Hash.get(strings, str);

    if (NULL != rcs)
    {
        if (--(rcs->_rc) <= 0)
        {
            #include<stdio.h>

            printf("gonna free %p %p %lu %s\n", rcs, rcs->_string, rcs->_rc, String.chars(rcs->_string));

            rc_string_t* to_remove = Hash.remove(strings, rcs->_string);
            String.free(to_remove->_string);
            rcs->_string = NULL;
            free(to_remove);
            rcs = NULL;
        }
    }
}

const strings_api_t Strings = {Create, Free, GetString, FreeString};





