#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


int main(int argc, char const *argv[])
{

    int fd = open("./driver1_output", O_CREAT | O_WRONLY, 0666);

    printf("got %d - %d\n", fd, errno);
    write(fd, "I am driver1, b0ss\n", 20);
    write(fd, argv[1], strlen(argv[1]));
    write(fd, "\n", 1);
    write(fd, argv[2], strlen(argv[2]));
    write(fd, "\n", 1);

    close(fd);
    
    return 0;
}
