#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/route.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>  
#include <fcntl.h> // for open
#include <unistd.h>

void addRoute(char * destination, char* mask ,char* gateway)
{
    int fd = socket( PF_INET, SOCK_DGRAM,  IPPROTO_IP);

    struct rtentry route;
    memset( &route, 0, sizeof( route ) );

    struct sockaddr_in * addr = (struct sockaddr_in*)&route.rt_gateway;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(gateway);

    addr = (struct sockaddr_in*) &route.rt_dst;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(destination);;

    addr = (struct sockaddr_in*) &route.rt_genmask;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(mask);//INADDR_NONE; //255.255.255.255
    route.rt_flags = RTF_UP | RTF_GATEWAY;
    //route.rt_metric = 0;

    if (ioctl( fd, SIOCADDRT, &route ) < 0)
    {
        printf("ioctl failed and returned errno %s %i \n", strerror(errno),errno);
    }
    close
    ( fd );
}

int main(int argc, char* argv[])
{
    // usage: distination, mask  and gateway
    if (argc == 4)
        addRoute(argv[1],argv[2],argv[3]);
    else{
        printf("Few parameters");
    }
    return 0;
}
