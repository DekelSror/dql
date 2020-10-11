#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>


static char write_buf[] = "yodelay-hee-hoo!!\n\n";

int main(void)
{
    int write_fd = open("dql_test_fifo", O_WRONLY);


    ssize_t written = write(write_fd, write_buf, 20);

    printf("wrote that\n");

    close(write_fd);
    
    return 0;
}
