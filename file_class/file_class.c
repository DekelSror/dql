#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "file_class.h"

typedef struct 
{
    int _fd;
    char* _map;
    size_t _offset;
    off_t file_size;
} _file_t;



static file_t Create(const char* file_name)
{
    const long page_size = sysconf(_SC_PAGE_SIZE);
    const int fd = open(file_name, O_CREAT);

    if (0 >= fd) return NULL;

    _file_t* file = malloc(sizeof *file + page_size);

    file->_fd = fd;
    file->_map = NULL;
    file->_offset = 0;

    return file;
}

static file_t OpenRO(const char* file_name)
{
    const int fd = open(file_name, O_RDONLY);

    if (0 >= fd) return NULL;

    struct stat file_stats;

    int stat_rc = fstat(fd, &file_stats);

    void* map = mmap(NULL, file_stats.st_size, PROT_READ, 0, fd, 0);

    _file_t* this = malloc(sizeof *this);

    this->file_size = file_stats.st_size;
    this->_map = map;
    this->_offset = 0;
    this->_fd = fd;



    return this;
}

#define file_thisify _file_t* this = (_file_t*)_this;

static char* Read(void* _this, size_t len)
{
    file_thisify

    if ((off_t)this->_offset >= this->file_size) return NULL;

    char* retval = this->_map + this->_offset;

    this->_offset += len;

    return retval;
}

const file_api_t File = {Create, OpenRO, Read};

