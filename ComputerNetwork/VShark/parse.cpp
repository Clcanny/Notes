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

uint16_t IpHeader::getDatagramLength()
{
    return datagramLength;
}

void IpHeader::toHost()
{
    datagramLength = ntohs(datagramLength);
    flagsFragmentationOffset = ntohs(flagsFragmentationOffset);
    srcIpAddress = ntohl(srcIpAddress);
    dstIpAddress = ntohl(dstIpAddress);

    /* uint16_t *p = &identifier + 1; */
    /* *p = ntohs(*p); */
    /* printf("fuck: %x\n", *p); */
    /* printf("MF: %u\n", flags & 0x1); */
    /* printf("fragmentationOffset: %u\n", fragmentationOffset); */
    /* printf("fragmentationOffset: %u\n", (*p) & 0x00ffffff); */
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
    assert ((flagsFragmentationOffset & 0x8000) == 0); 

    return ::check((uint16_t *)this, checksum, getDatagramLength());
}

bool IpHeader::getFlagDontFragment()
{
    return flagsFragmentationOffset & 0x4000;
}

bool IpHeader::getFlagMoreFragments()
{
    return flagsFragmentationOffset & 0x2000;
}

uint16_t IpHeader::getFragmentationOffset()
{
    return flagsFragmentationOffset & 0x1FFF;
}

int IpHeader::getHeaderLength()
{
    return headerLength << 4;
}

void IpHeader::print()
{
    printf("headerLength: %d, version: %d, identifier: %d, ttl: %d\n",
            headerLength, version, identifier, timeToLive);
    printf("Zero: %d, DF: %d, MF: %d, fragmentationOffset: %u\n",
            flagsFragmentationOffset & 0x8000,
            getFlagDontFragment(),
            getFlagMoreFragments(),
            getFragmentationOffset());
    printf("srcIp: %u.%u.%u.%u\n", (srcIpAddress & 0xff000000) >> 24,
                                   (srcIpAddress & 0x00ff0000) >> 16,
                                   (srcIpAddress & 0x0000ff00) >> 8,
                                   (srcIpAddress & 0x000000ff));
    printf("dstIp: %u.%u.%u.%u\n", (dstIpAddress & 0xff000000) >> 24,
                                   (dstIpAddress & 0x00ff0000) >> 16,
                                   (dstIpAddress & 0x0000ff00) >> 8,
                                   (dstIpAddress & 0x000000ff));
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
    assert (sizeof(UdpHeader) == 8);
    return ((uint8_t *)this) + sizeof(UdpHeader);
}
