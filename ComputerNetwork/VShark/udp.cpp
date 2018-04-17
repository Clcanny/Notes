#include "udp.h"
#include "helper.h"

#include <stdio.h>
#include <cassert>
#include <arpa/inet.h>

void UdpHeader::toHost()
{
    srcPort = ntohs(srcPort);
    dstPort = ntohs(dstPort);
    length = ntohs(length);
}

void UdpHeader::print()
{
    printf("source port: %d, destination port: %d, length: %d\n", srcPort, dstPort, length);
}

uint8_t *UdpHeader::getData()
{
    assert (sizeof(UdpHeader) == 8);
    return ((uint8_t *)this) + sizeof(UdpHeader);
}
