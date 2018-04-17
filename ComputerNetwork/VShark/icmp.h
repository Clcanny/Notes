#ifndef DEMONS_ICMP_H
#define DEMONS_ICMP_H

#include <inttypes.h>

class IcmpHeader
{
    public:
        uint8_t type;
        uint8_t code;
        uint16_t checksum;
        union
        {
            struct
            {
                uint16_t id;
                uint16_t sequence;
            };
            uint32_t gateway;
            struct
            {
                uint16_t unused;
                uint16_t mtu;
            };
        };

    public:
        void check(uint32_t length);
        void toHost();
        uint32_t print();
};

#endif
