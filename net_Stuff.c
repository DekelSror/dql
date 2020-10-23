#include <stdio.h>
#include <netdb.h>
#include <errno.h>

int main(void)
{
    struct sockaddr_in ser_addr;
    socklen_t salen = sizeof(struct sockaddr_in);
    char host_buf[NI_MAXHOST];
    char serv_buf[NI_MAXSERV];


    int s = socket(AF_INET, SOCK_STREAM, 0);

    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = INADDR_ANY;
    ser_addr.sin_port = htons(14444);


    const int bind_res = bind(s, (struct sockaddr*)&ser_addr, salen);

    if (bind_res)
    {
        printf("bind trouble %d errno %d \n", bind_res, errno);
    }

    int res = getnameinfo((const struct sockaddr*)&ser_addr, salen, host_buf, NI_MAXHOST, serv_buf, NI_MAXSERV, 0);

    if (res)
    {
        printf("trouble %d errno  %d\n", res, errno);
    }
    else
    {
        printf("%s\n %s\n", host_buf, serv_buf);
    }
    


    return 0;
}
