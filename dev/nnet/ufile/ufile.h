#ifndef dql_ufile_h
#define dql_ufile_h

#include <sys/stat.h>

typedef struct 
{
    int _fd;
    char* _map;
    struct stat _stat;
} ufile_t;


typedef struct 
{
    int (*create_st)(ufile_t* uf, const char* path);
    void(*free)(ufile_t*);
} ufile_api_t;

extern const ufile_api_t UFile;

#endif // dql_ufile_h
