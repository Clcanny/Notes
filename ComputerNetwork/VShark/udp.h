#ifndef DEMONS_UDP_H
#define DEMONS_UDP_H

#include <inttypes.h>

class UdpHeader
{
    public:
        uint16_t srcPort;
        uint16_t dstPort;

        uint16_t length;
        uint16_t checksum;

    public:
        void check(uint32_t length);
        void toHost();
        uint32_t print();
        uint8_t *getData();
};

#endif
