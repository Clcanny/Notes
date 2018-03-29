#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <iostream>

int main()
{
    int sd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);

    int len = 8;
    char payload[len];
    for (int i = 0; i < len; i++)
    {
        payload[i] = i + '\0';
    }

    struct hostent *hname = gethostbyname("172.0.0.2");
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = hname->h_addrtype;
    addr.sin_port = 0;
    addr.sin_addr.s_addr = *(long*)hname->h_addr;

    sendto(sd, payload, len, 0, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
    std::cout << "sending..." << std::endl;
}
