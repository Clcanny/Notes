#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <cassert>
#include <iostream>

int main()
{
    int sd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    struct sockaddr_in addr;
    int len = sizeof(addr);
    unsigned char buf[1024];
    while (true)
    {
        int bytes = recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr*)&addr, (socklen_t*)&len);
        assert (bytes > 0);
        std::cout << "received " << bytes << " bytes" << std::endl;
        std::cout << buf << std::endl;
    }
}
