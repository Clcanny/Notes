#include "mac.h"

#include <stdio.h>
#include <cassert>
#include <arpa/inet.h>

void MacHeader::check(uint32_t length)
{
    /* don't know generator G */
    assert (sizeof(MacHeader) == 14);
}

void MacHeader::toHost()
{
    type = ntohs(type);
}

uint32_t MacHeader::print()
{
    printf("srcAddress: %02x:%02x:%02x:%02x:%02x:%02x\n",
            srcAddress[0], srcAddress[1], srcAddress[2],
            srcAddress[3], srcAddress[4], srcAddress[5]);
    printf("dstAddress: %02x:%02x:%02x:%02x:%02x:%02x\n",
            dstAddress[0], dstAddress[1], dstAddress[2],
            dstAddress[3], dstAddress[4], dstAddress[5]);

    return sizeof(MacHeader);
}

uint8_t *MacHeader::getData()
{
    assert (sizeof(MacHeader) == 14);
    return ((uint8_t *)this) + sizeof(MacHeader);
}

bool MacHeader::isIp()
{
    return type == 0x0800;
}

bool MacHeader::isArp()
{
    return type == 0x0806;
}
