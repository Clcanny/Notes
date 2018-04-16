#include <unistd.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
#include <stdio.h>
#include <cassert>

#include "parse.h"

#define ETH_P_IP   0x0800 /* Internet Protocol packet     */
#define ETH_P_ARP  0x0806 /* Address Resolution packet    */
#define ETH_P_ALL  0x0003 /* Every packet (be careful!!!) */
#define BUFFER_MAX 2048 

int main()
{
    int sock_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    if (sock_fd < 0)
    {
        return -1;
    }

    char buffer[BUFFER_MAX];
    while(1)
    {
        /* buffer中不包含Preamble和CRC */
        int n_read = recvfrom(sock_fd, buffer, sizeof(buffer), 0, nullptr, nullptr);
        if (n_read < 42)
        {
            return -1;
        }

        /* mac中不包含Preamble和CRC */
        MacHeader *mac = (MacHeader *)buffer;
        mac->toHost();
        /* mac->printAddress(true); */
        /* mac->printAddress(false); */

        if (mac->isIp())
        {
            IpHeader *ip = (IpHeader *)(mac->getData());
            ip->toHost();
            /* ip->print(); */
            if (ip->getFlagMoreFragments() == 0 && ip->getFragmentationOffset() == 0)
            {
                assert (ip->getDatagramLength() == n_read - sizeof(MacHeader));
            }
            else
            {
                /* To allow the destination host to perform these reassembly tasks,
                 * the designers of IP (version 4) put identification, flag, and fragmentation offset
                 * fields in the IP datagram header.
                 *
                 * When a datagram is created, the sending host stamps the datagram with an identification
                 * number as well as source and destination addresses. Typically, the sending host increments the
                 * identification number for each datagram it sends. When a router needs to fragment a
                 * datagram, each resulting datagram (that is, fragment) is stamped with the source
                 * address, destination address, and identification number of the original datagram.
                 *
                 * When the destination receives a series of datagrams from the same sending host, it
                 * can examine the identification numbers of the datagrams to determine which of the
                 * datagrams are actually fragments of the same larger datagram.
                 *
                 * Because IP is an unreliable service, one or more of the fragments may never arrive
                 * at the destination. For this reason, in order for the destination host to be absolutely
                 * sure it has received the last fragment of the original datagram, the last fragment
                 * has a flag bit set to 0, whereas all the other fragments have this flag bit set to 1.
                 *
                 * Also, in order for the destination host to determine whether a fragment is missing (and also
                 * to be able to reassemble the fragments in their proper order), the offset field is used to specify
                 * where the fragment fits within the original IP datagram.
                 */
                /* printf("fragmentationOffset: %d\n", ip->getFragmentationOffset()); */
                /* assert (false); */
            }

            switch (ip->upperLayerProtocol)
            {
                case IPPROTO_ICMP:
                {
                    IcmpHeader *icmp = (IcmpHeader *)ip->getData();
                    icmp->toHost();
                    /* icmp->print(); */
                    break;
                }

                case IPPROTO_UDP:
                {
                    /* printf("udp\n"); */
                    UdpHeader *udp = (UdpHeader *)(ip->getData());
                    udp->toHost();
                    /* udp->print(); */
                    break;
                }

                case IPPROTO_TCP:
                {
                    /* printf("tcp\n"); */
                    TcpHeader *tcp = (TcpHeader *)(ip->getData());
                    tcp->toHost();
                    tcp->print();
                    printf("\n");
                    tcp->getData();
                    break;
                }
            }
        }
        else if (mac->isArp())
        {
            /* printf("arp\n"); */
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
