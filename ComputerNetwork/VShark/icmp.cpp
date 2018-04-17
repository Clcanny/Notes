#include "icmp.h"

#include <stdio.h>
#include <cassert>
#include <arpa/inet.h>

void IcmpHeader::toHost()
{
    id = ntohs(id);
    sequence = ntohs(sequence);
}

void IcmpHeader::print()
{
    printf("type: %d, code: %d, id: %d, sequence: %d\n", type, code, id, sequence);
}
