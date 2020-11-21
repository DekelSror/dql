#include <unistd.h>

int SerializeTo(void* obj, unsigned size, int fd)
{
    ssize_t written = write(fd, obj, size);

    return written == size;
}

int DeserializeFrom(int fd, unsigned size, void* buf)
{
    return read(fd, buf, size) == size;
}


typedef struct dood
{
    /* data */
};
