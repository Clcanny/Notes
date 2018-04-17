#ifndef DEMONS_ARP_HEADER
#define DEMONS_ARP_HEADER

#include <inttypes.h>

class ArpHeader
{
    public:
        uint16_t hardwareType;
        uint16_t protocolType;
        uint8_t hardwareAddressLength;
        uint8_t protocolAddressLength;
        uint16_t operation;
        uint8_t senderHardwareAddress[6];
        uint8_t senderProtocolAddress[4];
        uint8_t targetHardwareAddress[6];
        uint8_t targetProtocolAddress[4];

    public:
        void toHost();
        void print();
};

#endif
