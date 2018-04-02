#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cassert>
#include <iostream>
#include <iomanip>

int main()
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    /* IPv4 */
    hints.ai_family = AF_INET;

    struct addrinfo *addrinfo;
    getaddrinfo("localhost", nullptr, &hints, &addrinfo);
    assert (addrinfo != nullptr);
    uint32_t addr = ((struct sockaddr_in*)(addrinfo->ai_addr))->sin_addr.s_addr;
    std::cout << std::hex << std::setw(8) << std::setfill('0') << addr << std::endl;
    freeaddrinfo(addrinfo);

    addr = (127 << 24) | 1;
    uint16_t high = addr >> 16;
    uint16_t low = addr & 0x0000ffff;
    high = htons(high);
    low = htons(low);
    addr = (high << 16) | low;
    std::cout << std::hex << std::setw(8) << std::setfill('0') << addr << std::endl;

    struct sockaddr_in sa;
    /* presentation to network */
    inet_pton(AF_INET, "127.0.0.1", &(sa.sin_addr));
    addr = sa.sin_addr.s_addr;
    std::cout << std::hex << std::setw(8) << std::setfill('0') << addr << std::endl;

    char presentation[INET_ADDRSTRLEN];
    /* network to presentation */
    inet_ntop(AF_INET, &(sa.sin_addr), presentation, INET_ADDRSTRLEN);
    std::cout << presentation << std::endl;
}
