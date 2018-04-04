#include "parse.h"

#include <stdio.h>
#include <cassert>
#include <arpa/inet.h>

void MacHeader::toHost()
{
    type = ntohs(type);
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

bool MacHeader::isIp()
{
    return type == 0x0800;
}

bool MacHeader::isArp()
{
    return type == 0x0806;
}

bool check(uint16_t *data, uint16_t checksum, int length)
{
    uint32_t sum = 0;
    int i;
    for (i = length; i > 1; i -= 2)
    {
        sum += *data;
        data++;
    }
    if (i == 1)
    {
        uint8_t *p = (uint8_t *)data;
        p++;
        sum += *p;
    }

    while (sum > 0xFFFF)
    {
        sum = (sum >> 16) + (sum & 0xFFFF);
    }

    /* sum += checksum; */
    /* printf("%d\n", sum); */

    return (sum & 0xFFFF) == 0;
}

int IpHeader::getHeaderLength()
{
    return headerLength << 2;
}

uint16_t IpHeader::getDatagramLength()
{
    return ntohs(datagramLength);
}

bool IpHeader::isIcmp()
{
    /* IANA Protocol Numbers 2012 */
    return upperLayerProtocol == 1;
}

uint8_t *IpHeader::getData()
{
    assert (sizeof(IpHeader) == 20);
    return ((uint8_t *)this) + getHeaderLength();
}

bool IpHeader::check()
{
    uint16_t checksum = headerChecksum;
    /* headerChecksum = 0; */
    assert ((flags & 4) == 0); 

    return ::check((uint16_t *)this, checksum, getDatagramLength());
}

bool IpHeader::getFlagDontFragment()
{
    return flags >> 1;
}

bool IpHeader::getFlagMoreFragments()
{
    return flags & 1;
}

void IpHeader::print()
{
    printf("headerLength: %d, version: %d, identifier: %d, ttl: %d\n",
            headerLength, version, identifier, timeToLive);
}

void IcmpHeader::print()
{
    printf("type: %d, code: %d, id: %d, sequence: %d\n", type, code, id, sequence);
}

void IcmpHeader::toHost()
{
    id = ntohs(id);
    sequence = ntohs(sequence);
}

void UdpHeader::toHost()
{
    srcPort = ntohs(srcPort);
    dstPort = ntohs(dstPort);
}

void UdpHeader::print()
{
    printf("source port: %d, destination port: %d, length: %d\n", srcPort, dstPort, length);
}

uint8_t *UdpHeader::getData()
{
    assert (sizeof(Udpheader) == 8);
    return ((uint8_t *)this) + sizeof(UdpHeader);
}
