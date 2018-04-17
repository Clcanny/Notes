#include "ip.h"
#include "helper.h"

#include <stdio.h>
#include <cassert>
#include <arpa/inet.h>

void IpHeader::check(uint32_t length)
{
    assert (sizeof(IpHeader) == 20);
    
    if (getFlagMoreFragments() == 0 && getFragmentationOffset() == 0)
    {
        assert (getDatagramLength() == length);
    }
    else
    {
        /* To allow the destination host to perform these reassembly tasks,
        * the designers of IP (version 4) put identification, flag, and fragmentation offset
        * fields in the IP datagram header.
        *
        * When a datagram is created, the sending host stamps the datagram with an identification
        * number as well as source and destination addresses. Typically, the sending host increments the
        * identification number for each datagram it sends. When a router needs to fragment a
        * datagram, each resulting datagram (that is, fragment) is stamped with the source
        * address, destination address, and identification number of the original datagram.
        *
        * When the destination receives a series of datagrams from the same sending host, it
        * can examine the identification numbers of the datagrams to determine which of the
        * datagrams are actually fragments of the same larger datagram.
        *
        * Because IP is an unreliable service, one or more of the fragments may never arrive
        * at the destination. For this reason, in order for the destination host to be absolutely
        * sure it has received the last fragment of the original datagram, the last fragment
        * has a flag bit set to 0, whereas all the other fragments have this flag bit set to 1.
        *
        * Also, in order for the destination host to determine whether a fragment is missing (and also
        * to be able to reassemble the fragments in their proper order), the offset field is used to specify
        * where the fragment fits within the original IP datagram.
        */
    }

    uint16_t checksum = headerChecksum;
    /* headerChecksum = 0; */
    assert ((flagsFragmentationOffset & 0x8000) == 0); 

    ::check((uint16_t *)this, checksum, getDatagramLength());
}

void IpHeader::toHost()
{
    datagramLength = ntohs(datagramLength);
    flagsFragmentationOffset = ntohs(flagsFragmentationOffset);
    srcIpAddress = ntohl(srcIpAddress);
    dstIpAddress = ntohl(dstIpAddress);
}

uint32_t IpHeader::print()
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

    return getHeaderLength();
}

uint8_t *IpHeader::getData()
{
    assert (sizeof(IpHeader) == 20);
    return ((uint8_t *)this) + getHeaderLength();
}

uint32_t IpHeader::getHeaderLength()
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
