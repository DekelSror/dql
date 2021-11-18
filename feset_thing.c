
#include <sys/select.h> // select, fd_set and Co.
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


int main()
{
    fd_set set;

    FD_ZERO(&set);


    FD_SET(1, &set);
    FD_SET(2, &set);
    FD_SET(3, &set);

    printf("%d\n", FD_ISSET(1, &set));
    printf("%d\n", FD_ISSET(4, &set));
    printf("%d\n", FD_ISSET(1, &set));
    FD_SETSIZE;

    return 0;
}
