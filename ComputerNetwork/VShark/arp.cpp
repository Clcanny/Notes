#include "arp.h"

#include <stdio.h>
#include <cassert>
#include <arpa/inet.h>

void ArpHeader::check(uint32_t length)
{
    assert (sizeof(ArpHeader) == 28);

    /* Ethernet */
    assert (hardwareType == htons(1));
    /* IPv4 */
    assert (protocolType == htons(0x0800));
}

void ArpHeader::toHost()
{
    hardwareType = ntohs(hardwareType);
    protocolType = ntohs(protocolType);
    operation = ntohs(operation);

    assert (hardwareType == 1);
    assert (protocolType == 0x0800);
}

uint32_t ArpHeader::print()
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

    return sizeof(ArpHeader);
}
