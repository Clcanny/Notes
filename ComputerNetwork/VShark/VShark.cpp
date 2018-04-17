#include <unistd.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
#include <stdio.h>
#include <cassert>

#include "mac.h"
#include "arp.h"
#include "ip.h"
#include "icmp.h"
#include "udp.h"
#include "tcp.h"

#define ETH_P_IP   0x0800 /* Internet Protocol packet     */
#define ETH_P_ARP  0x0806 /* Address Resolution packet    */
#define ETH_P_ALL  0x0003 /* Every packet (be careful!!!) */
#define BUFFER_MAX 2048 

int main()
{
    int sock_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock_fd < 0)
    {
        return -1;
    }

    char buffer[BUFFER_MAX];
    while(1)
    {
        /* buffer中不包含Preamble和CRC */
        uint32_t n_read = recvfrom(sock_fd, buffer, sizeof(buffer), 0, nullptr, nullptr);
        if (n_read < 42)
        {
            return -1;
        }

        /* mac中不包含Preamble和CRC */
        MacHeader *mac = (MacHeader *)buffer;
        mac->toHost();
        mac->check(n_read);
        n_read -= mac->print();

        if (mac->isIp())
        {
            IpHeader *ip = (IpHeader *)(mac->getData());
            ip->toHost();
            ip->check(n_read);
            n_read -= ip->print();

            switch (ip->upperLayerProtocol)
            {
                case IPPROTO_ICMP:
                {
                    IcmpHeader *icmp = (IcmpHeader *)ip->getData();
                    icmp->toHost();
                    icmp->check(n_read);
                    n_read -= icmp->print();
                    break;
                }

                case IPPROTO_UDP:
                {
                    UdpHeader *udp = (UdpHeader *)(ip->getData());
                    udp->toHost();
                    udp->check(n_read);
                    n_read -= udp->print();
                    break;
                }

                case IPPROTO_TCP:
                {
                    TcpHeader *tcp = (TcpHeader *)(ip->getData());
                    tcp->toHost();
                    n_read -= tcp->print();
                    break;
                }
            }
        }
        else if (mac->isArp())
        {
            ArpHeader *arpHeader = (ArpHeader *)(mac->getData());
            arpHeader->toHost();
            arpHeader->print();
        }
        else
        {
            continue;
        }

        /* char *eth_head = buffer; */
        /* char *p = eth_head; */
        
        /* printf("MAC address: %.2x:%02x:%02x:%02x:%02x:%02x" */
        /*        "==> %.2x:%02x:%02x:%02x:%02x:%02x\n", */
        /*        p[6], p[7], p[8], p[9], p[10], p[11], */
        /*        p[0], p[1], p[2], p[3], p[4], p[5]); */
        
        /* char *ip_head = eth_head + 14; */
        /* p = ip_head + 12; */
        /* printf("IP:%d.%d.%d.%d==> %d.%d.%d.%d\n", */
        /*         p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7]); */
        /* int proto = (ip_head + 9)[0]; */
        
        /* p = ip_head +12; */
        /* printf("Protocol:"); */
        /* switch (proto) */
        /* { */
        /*     case IPPROTO_ICMP:printf("icmp\n");break; */
        /*     case IPPROTO_IGMP:printf("igmp\n");break; */
        /*     case IPPROTO_IPIP:printf("ipip\n");break; */
        /*     case IPPROTO_TCP:printf("tcp\n");break; */
        /*     case IPPROTO_UDP:printf("udp\n");break; */
        /*     default:printf("Pls query yourself\n"); */
        /* } */
    }
    return -1;
}
