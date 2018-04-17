#ifndef DEMONS_TCP_H
#define DEMONS_TCP_H

#include <inttypes.h>

class TcpHeader
{
    public:
        uint16_t srcPort;
        uint16_t dstPort;
        
        uint32_t sequenceNumber;
        uint32_t ackNumber;
        
        uint16_t mixFields;
        uint16_t receiveWindow;

        uint16_t internetChecksum;
        uint16_t urgentDataPointer;

        /* don't support options */

    public:
        bool check(int length);
        void toHost();
        void print();
        uint8_t *getData();

        uint16_t getHeaderLength();
        uint8_t getFlags();
};

#endif
