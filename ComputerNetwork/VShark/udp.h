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
        bool check();
        void toHost();
        void print();
        uint8_t *getData();
};

#endif
