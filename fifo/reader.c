#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>


static char readbuf[4097] = {0};

int main(void)
{
    int res = mkfifo("dql_test_fifo", 0777);

    if (res) printf("mkfifo says %d", res);

    int read_fd = open("dql_test_fifo", O_RDONLY);


    ssize_t have_read = read(read_fd, readbuf, sizeof(readbuf) - 1);

    printf("%s\n", readbuf);
    
    close(read_fd);
    
    return 0;
}
