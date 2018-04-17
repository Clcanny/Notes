#include "tcp.h"

#include <stdio.h>
#include <cassert>
#include <arpa/inet.h>

void TcpHeader::toHost()
{
    srcPort = ntohs(srcPort);
    dstPort = ntohs(dstPort);
    sequenceNumber = ntohl(sequenceNumber);
    ackNumber = ntohl(ackNumber);

    mixFields = ntohs(mixFields);
    assert ((mixFields & 0x0FC0) == 0);

    receiveWindow = ntohs(receiveWindow);
    urgentDataPointer = ntohs(urgentDataPointer);
}

uint32_t TcpHeader::print()
{
    printf("srcPort: %u, dstPort: %u\n", srcPort, dstPort);
    printf("sequenceNumber: %u, ackNumber: %u\n", sequenceNumber, ackNumber);
    printf("headerLength: %u\n", getHeaderLength());
    printf("receiveWindow: %u, urgentDataPointer: %u\n", receiveWindow, urgentDataPointer);
    printf("urg:%u, ack: %d, psh: %u, rst: %u, syn: %u, fin: %u\n",
            (mixFields & 0x0020) >> 5,
            (mixFields & 0x0010) >> 4,
            (mixFields & 0x0008) >> 3,
            (mixFields & 0x0004) >> 2,
            (mixFields & 0x0002) >> 1,
            (mixFields & 0x0001)
          );

    return getHeaderLength();
}

uint8_t *TcpHeader::getData()
{
    assert (sizeof(TcpHeader) == 20);
    return (uint8_t *)(this) + getHeaderLength();
}

uint16_t TcpHeader::getHeaderLength()
{
    /* 12 - 2 = 10 */
    return (mixFields & 0xF000) >> 10;
}

uint8_t TcpHeader::getFlags()
{
    return mixFields & 0x003F;
}
