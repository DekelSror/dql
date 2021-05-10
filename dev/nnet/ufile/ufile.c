#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

#include "ufile.h"



static void Free(ufile_t* uf)
{
    close(uf->_fd);
    munmap(uf->_map, uf->_stat.st_size);
}

static int CreateStatic(ufile_t* uf, const char* path)
{
    int fd = open(path, O_RDONLY);   

    if (fd == -1) return 1; 

    struct stat s;
    fstat(fd, &s);

    char* map = mmap(NULL, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map == NULL)
    {
        close(fd);
        return 2;
    }

    uf->_fd = fd;
    uf->_map = map;
    memmove(&(uf->_stat), &s, sizeof(struct stat));

    return 0;
}

const ufile_api_t UFile = {CreateStatic, Free};

