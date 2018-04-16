#ifndef PARSE_H
#define PARSE_H

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
        void toHost();
        bool check(int length);
        uint8_t *getData();
        void printAddress(bool isSrc);
        bool isIp();
        bool isArp();
};

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
        /* IPPROTO_ICMP */
        uint8_t upperLayerProtocol;
        uint16_t headerChecksum;

        uint32_t srcIpAddress;
        uint32_t dstIpAddress;

        /* 不支持options */

    public:
        int getHeaderLength();
        uint16_t getDatagramLength();
        bool isIcmp();
        uint8_t *getData();
        bool check();
        bool getFlagDontFragment();
        bool getFlagMoreFragments();
        uint16_t getFragmentationOffset();
        void print();
        void toHost();
};

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
        void print();
        void toHost();
};

class UdpHeader
{
    public:
        uint16_t srcPort;
        uint16_t dstPort;

        uint16_t length;
        uint16_t checksum;

    public:
        bool check();
        void print();
        void toHost();
        uint8_t *getData();
};

class TcpHeader
{
    public:
        uint16_t srcPort;
        uint16_t dstPort;
        
        uint32_t sequenceNumber;
        uint32_t ackNumber;
        
        unsigned headerLength:4;
        unsigned unused:6;
        bool urg;
        bool ack;
        bool psh;
        bool rst;
        bool syn;
        bool fin;
        uint16_t receiveWindow;

        uint16_t internetChecksum;
        uint16_t urgentDataPointer;

        /* don't support options */

    public:
        bool check(int length);
        uint8_t *getData();
};

class HttpHeader
{
};

#endif
