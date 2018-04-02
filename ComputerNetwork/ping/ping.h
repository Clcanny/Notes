#ifndef PING_H
#define PING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <setjmp.h>
#include <errno.h>

#include <string>
#include <vector>

#include <cassert>

#define PACKET_SIZE 4096
#define MAX_WAIT_TIME 5
#define MAX_NO_PACKETS 3

struct IcmpEchoReply
{
    int icmpSeq;
    int icmpLen;
    int ipTtl;
    double rtt;
    std::string fromAddr;
    bool isReply;
};

struct PingResult
{
    int dataLen;
    int nsend;
    int nreceived;
    std::string ip;
    std::string error;
    std::vector<IcmpEchoReply> icmpEchoReplys;
};

/* POD */
class IpHeader
{
    public:
        /* BYTE_ORDER == BIG_ENDIAN */
        unsigned headerLength:4;
        unsigned version:4;

        uint8_t typeOfService;
        uint16_t datagramLength;
        uint16_t identifier;
        unsigned flags:3;
        unsigned fragmentationOffset:13;

        uint8_t timeToLive;
        uint8_t upperLayerProtocol;
        uint16_t headerChecksum;

        uint32_t srcIpAddress;
        uint32_t dstIpAddress;

        /* 不支持options */

    public:
        int getHeaderLength();
        bool isIcmp();
        uint8_t *getData();
        bool check();
};

/* POD */
class IcmpHeader
{
    public:
        uint8_t type;
        uint8_t code;
        uint16_t checksum;
        union
        {
            struct
            {
                uint16_t id;
                uint16_t sequence;
            };
            uint32_t gateway;
            struct
            {
                uint16_t unused;
                uint16_t mtu;
            };
        };

    public:
        bool check(int length);
        uint8_t *getData();
};

class Ping
{
    private:
        char m_sendpacket[PACKET_SIZE];
        char m_recvpacket[PACKET_SIZE];
        int m_maxPacketSize;
        int m_sockfd;
        int m_datalen;
        int m_nsend;
        int m_nreceived;
        int m_icmp_seq;
        struct sockaddr_in m_dest_addr;
        struct sockaddr_in m_from_addr;
        pid_t m_pid;

    public:
        Ping();
        bool ping(std::string host, PingResult &pingResult);
        bool ping(std::string host, int count, PingResult& pingResult);

    private:
        uint16_t getChecksum(uint16_t *addr, int len);
        int packIcmp(int pack_no, IcmpHeader *icmp);
        bool unpackIcmp(char *buf,int len, struct IcmpEchoReply *icmpEchoReply);
        struct timeval tvSub(struct timeval timeval1,struct timeval timval2);
        bool getsockaddr(const char * hostOrIp, sockaddr_in* sockaddr);
        bool sendPacket();
        bool recvPacket(PingResult &pingResult);
};

#endif
