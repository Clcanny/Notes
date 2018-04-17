#ifndef DEMONS_MAC_H
#define DEMONS_MAC_H

#include <inttypes.h>

class MacHeader
{
    public:
        uint8_t dstAddress[6];
        uint8_t srcAddress[6];

        /* 0x0800 IPv4
         * 0x0806 ARP
         * 0x0842 Wake-on-LAN
         * https://en.wikipedia.org/wiki/EtherType
         */
        uint16_t type;

    public:
        bool check(int length);
        void toHost();
        void print();
        uint8_t *getData();
        bool isIp();
        bool isArp();
};

#endif
