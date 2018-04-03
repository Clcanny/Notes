#include <unistd.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
#include <iostream>

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
        int n_read = recvfrom(sock_fd, buffer, sizeof(buffer), 0, nullptr, nullptr);
        if (n_read < 42)
        {
            return -1;
        }
        MacHeader *mac = (MacHeader *)buffer;
        mac->toHost();
        mac->printAddress(true);
        mac->printAddress(false);
        /* printf("MAC address: %.2x:%02x:%02x:%02x:%02x:%02x" */
        /*        "==> %.2x:%02x:%02x:%02x:%02x:%02x\n", */
        /*        mac->srcAddress[0], mac->srcAddress[1], mac->srcAddress[2], mac->srcAddress[3], mac->srcAddress[4], mac->srcAddress[5], */
        /*        mac->dstAddress[0], mac->dstAddress[1], mac->dstAddress[2], mac->dstAddress[3], mac->dstAddress[4], mac->dstAddress[5]); */

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
