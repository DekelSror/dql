#include <stdio.h>
#include <unistd.h>
#include <string.h>
// #include <sys/types.h>
#include <inttypes.h>

int SerializeTo(void* obj, unsigned size, int fd)
{
    ssize_t written = write(fd, obj, size);

    return written == size;
}

int DeserializeFrom(int fd, unsigned size, void* buf)
{
    return read(fd, buf, size) == size;
}
