#include "ip.h"
#include "helper.h"

#include <stdio.h>
#include <cassert>
#include <arpa/inet.h>

bool IpHeader::check()
{
    uint16_t checksum = headerChecksum;
    /* headerChecksum = 0; */
    assert ((flagsFragmentationOffset & 0x8000) == 0); 

    return ::check((uint16_t *)this, checksum, getDatagramLength());
}

void IpHeader::toHost()
{
    datagramLength = ntohs(datagramLength);
    flagsFragmentationOffset = ntohs(flagsFragmentationOffset);
    srcIpAddress = ntohl(srcIpAddress);
    dstIpAddress = ntohl(dstIpAddress);
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

uint8_t *IpHeader::getData()
{
    assert (sizeof(IpHeader) == 20);
    return ((uint8_t *)this) + getHeaderLength();
}

int IpHeader::getHeaderLength()
{
    return headerLength << 2;
}

uint16_t IpHeader::getDatagramLength()
{
    return datagramLength;
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

bool IpHeader::isIcmp()
{
    /* IANA Protocol Numbers 2012 */
    return upperLayerProtocol == 1;
}
