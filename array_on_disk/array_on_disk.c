#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "array_on_disk.h"




static array_on_disk_t Create(size_t length, unsigned element_size)
{
    aod_filedata_t* aod = malloc(sizeof(*aod) + length * element_size);

    aod->_length = length;
    aod->_element_size = element_size;
    memset(aod->_array, 0, length * element_size);

    return aod;
}


static array_on_disk_t FromFile(int fd)
{
    struct stat file_stat;
    fstat(fd, &file_stat);

    aod_filedata_t* arr = mmap(NULL, file_stat.st_size, PROT_READ, 0, fd, 0);    

    return arr;
}

static size_t Length(const array_on_disk_t _this) {const aod_filedata_t* this = _this; return this->_length;}

static void* At(array_on_disk_t _this, size_t idx) 
{
    aod_filedata_t* this = _this; 
    return this->_array + (idx * this->_element_size);
}

const array_on_disk_api_t Array_on_disk = {Create, FromFile, Length, At}; 
