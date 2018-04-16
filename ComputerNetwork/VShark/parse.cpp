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

void ArpHeader::toHost()
{
    hardwareType = ntohs(hardwareType);
    protocolType = ntohs(protocolType);
    operation = ntohs(operation);

    /* Ethernet */
    assert (hardwareType == 1);
    /* IPv4 */
    assert (protocolType == 0x0800);
}

void ArpHeader::print()
{
    printf("senderHardwareAddress: %02x:%02x:%02x:%02x:%02x:%02x\n",
            senderHardwareAddress[0], senderHardwareAddress[1],
            senderHardwareAddress[2], senderHardwareAddress[3],
            senderHardwareAddress[4], senderHardwareAddress[5]);

    printf("senderProtocolAddress: %d.%d.%d.%d\n",
            senderProtocolAddress[0], senderProtocolAddress[1],
            senderProtocolAddress[2], senderProtocolAddress[3]);

    printf("targetHardwareAddress: %02x:%02x:%02x:%02x:%02x:%02x\n",
            targetHardwareAddress[0], targetHardwareAddress[1],
            targetHardwareAddress[2], targetHardwareAddress[3],
            targetHardwareAddress[4], targetHardwareAddress[5]);

    printf("targetProtocolAddress: %d.%d.%d.%d\n",
            targetProtocolAddress[0], targetProtocolAddress[1],
            targetProtocolAddress[2], targetProtocolAddress[3]);
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
    return headerLength << 2;
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

void TcpHeader::toHost()
{
    srcPort = ntohs(srcPort);
    dstPort = ntohs(dstPort);
    sequenceNumber = ntohl(sequenceNumber);
    ackNumber = ntohl(ackNumber);

    /* uint16_t *p = &receiveWindow - 1; */
    /* *p = ntohs(*p); */
    /* assert (unused == 0); */
    mixFields = ntohs(mixFields);
    assert ((mixFields & 0x0FC0) == 0);

    receiveWindow = ntohs(receiveWindow);
    urgentDataPointer = ntohs(urgentDataPointer);
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

void TcpHeader::print()
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
}

uint8_t *TcpHeader::getData()
{
    assert (sizeof(TcpHeader) == 20);
    return (uint8_t *)(this) + sizeof(TcpHeader);
}
