#ifndef DEMONS_IP_H
#define DEMONS_IP_H

#include <inttypes.h>

class IpHeader
{
    public:
        /* BYTE_ORDER == BIG_ENDIAN */
        unsigned headerLength:4;
        unsigned version:4;

        uint8_t typeOfService;
        uint16_t datagramLength;

        uint16_t identifier;

        /* 0 DF MF fragmentOffset */
        uint16_t flagsFragmentationOffset;

        uint8_t timeToLive;
        uint8_t upperLayerProtocol;
        uint16_t headerChecksum;

        uint32_t srcIpAddress;
        uint32_t dstIpAddress;

        /* 不支持options */

    public:
        void check(uint32_t length);
        void toHost();
        uint32_t print();
        uint8_t *getData();

        uint32_t getHeaderLength();
        uint16_t getDatagramLength();
        bool getFlagDontFragment();
        bool getFlagMoreFragments();
        uint16_t getFragmentationOffset();
        bool isIcmp();
};

#endif
