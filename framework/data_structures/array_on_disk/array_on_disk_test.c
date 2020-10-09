#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "array_on_disk.h"


unsigned vals[] = {943, 000002, 48333332, 0x40000, 2};

void WriteArrayToFileViaMap(void)
{
    struct stat file_stat;


    int array_file = open("aod_test", O_RDWR | O_APPEND | O_CREAT | O_TRUNC);
    fchmod(array_file, 0777);

    fstat(array_file, &file_stat);

    if (0 == file_stat.st_size)
    {
        ftruncate(array_file, 4096);
    }

    void* _map = mmap(NULL, file_stat.st_size ?file_stat.st_size : 4096, PROT_WRITE, MAP_SHARED, array_file, 0);
    char* map = _map;

    printf("file is %lu bytes big. fd %d. map at %p\n\n", file_stat.st_size, array_file, _map);

    size_t l = 5;
    unsigned lsize = 4;

    memmove(map, &l, 8);
    memmove(map + 8, &lsize, 4);
    memmove(map + 12, vals, sizeof(vals));

    aod_filedata_t* test = (aod_filedata_t*)map;

    printf("map %lu %u %d\n", *(size_t*)map, *(unsigned*)(map + 8), *(int*)(map + 12));

    printf("obj %lu, %u, %d\n", test->_length, test->_element_size, *(unsigned*)(test->_array));

    fstat(array_file, &file_stat);
    munmap(_map, file_stat.st_size);
    close(array_file);
}

int main(void)
{
    WriteArrayToFileViaMap();
    struct stat file_stat;
    int array_file = open("aod_test", O_RDONLY);

    fstat(array_file, &file_stat);

    void* _map = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, array_file, 0);
    aod_filedata_t* map = _map;


    printf("from file: %lu %u %d \n\n", map->_length, map->_element_size, *(unsigned*)map->_array);

    printf("idx 2 (should be 48333332) %d\n", *(unsigned*)Array_on_disk.at(map, 2));

    return 0;
} 
