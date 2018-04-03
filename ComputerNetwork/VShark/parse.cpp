#include "parse.h"

#include <stdio.h>
#include <cassert>
#include <arpa/inet.h>

void MacHeader::toHost()
{
    uint16_t *p = (uint16_t *)this;
    for (int i = 0; i < 7; i++)
    {
        *p = ntohs(*p);
    }
}

bool MacHeader::check(int length)
{
    /* don't know generator G */
    return true;
}

uint8_t *MacHeader::getData()
{
    assert (sizeof(MacHeader) == 14);
    return ((uint8_t *)this) + sizeof(MacHeader);
}

void MacHeader::printAddress(bool isSrc)
{
    uint8_t *address;
    if (isSrc)
    {
        address = srcAddress;
    }
    else
    {
        address = dstAddress;
    }

    printf("%02x-%02x-%02x-%02x-%02x-%02x\n", 
           address[0], address[1], address[2],
           address[3], address[4], address[5]);
}
