#include "icmp.h"

#include <stdio.h>
#include <cassert>
#include <arpa/inet.h>

void check(uint32_t length)
{
    assert (sizeof(IcmpHeader) == 8);
}

void IcmpHeader::toHost()
{
    id = ntohs(id);
    sequence = ntohs(sequence);
}

uint32_t IcmpHeader::print()
{
    printf("type: %d, code: %d, id: %d, sequence: %d\n", type, code, id, sequence);
    return sizeof(IcmpHeader);
}
