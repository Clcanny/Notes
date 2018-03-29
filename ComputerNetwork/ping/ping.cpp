#include <time.h>
#include <unistd.h>
#include <cassert>
#include <stdio.h>

#include <fcntl.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

class IcmpHeader
{
    private:
        static uint16_t sequenceIndex;

    public:
        uint8_t type;
        uint8_t code;
        uint16_t checksum;
        uint16_t identifier;
        uint16_t sequenceNumber;
        uint64_t timestamp;

    private:
        void makeChecksum()
        {
            uint16_t* buf = reinterpret_cast<uint16_t*>(this);
            int len = 1 + 1 + 2 + 2 + 2 + 8;
            unsigned int sum = 0;

            for (; len > 1; len -= 2)
            {
                sum += (*buf)++;
            }
            if (len == 1)
            {
                sum += *reinterpret_cast<uint8_t*>(buf);
            }

            sum = (sum >> 16) + (sum & 0xFFFF);
            sum += (sum >> 16);

            checksum = ~sum;
        }

        void display(void *buf, int bytes)
        {
            /* struct iphdr *ip = (struct iphdr*)buf; */
            /* struct icmphdr *icmp = buf + ip->ihl * 4; */
            
            /* printf("----------------\n"); */
            /* for (int i = 0; i < bytes; i++) */
            /* { */
            /* 	if (!(i & 15)) printf("\nX:  %d", i); */
            /* 	printf("X %c", ((unsigned char*)buf)[i]); */
            /* } */
            /* printf("\n"); */
            /* printf("IPv%d: hdr-size=%d pkt-size=%d protocol=%d TTL=%d src=%s ", */
            /* 	ip->version, ip->ihl*4, ntohs(ip->tot_len), ip->protocol, */
            /* 	ip->ttl, inet_ntoa(ip->saddr)); */
            /* printf("dst=%s\n", inet_ntoa(ip->daddr)); */
            /* if ( icmp->un.echo.id == pid ) */
            /* { */
            /* 	printf("ICMP: type[%d/%d] checksum[%d] id[%d] seq[%d]\n", */
            /* 		icmp->type, icmp->code, ntohs(icmp->checksum), */
            /* 		icmp->un.echo.id, icmp->un.echo.sequence); */
            /* } */
        }

        void makeRequestHeader()
        {
            type = 8;
            code = 0;
            checksum = 0;

            pid_t processId = getpid();
            identifier = *reinterpret_cast<uint16_t*>(&processId);

            sequenceNumber = sequenceIndex;
            sequenceIndex++;

            time_t t;
            time(&t);
            timestamp = *reinterpret_cast<uint64_t*>(&t);

            makeChecksum();
        }

    public:
        void request(struct sockaddr_in* addr)
        {
            makeRequestHeader();

            const int val = 255;

            /* int socket(int domain, int type, int protocol); */
            /* int sd = socket(PF_INET, SOCK_RAW, proto->p_proto); */
            int sd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
            assert (sd >= 0);
            /* getsockopt() and setsockopt() manipulate options for the socket referred to by the file descriptor sockfd.
             * Options may exist at multiple protocol levels; they are always present at the uppermost socket level.
             */
            assert (setsockopt(sd, SOL_IP, IP_TTL, &val, sizeof(val)) == 0);
            assert (fcntl(sd, F_SETFL, O_NONBLOCK) == 0);

            assert (sendto(sd, this, sizeof(IcmpHeader), 0, (struct sockaddr*)addr, sizeof(struct sockaddr_in)) > 0);
        }

        void response()
        {
            unsigned char buf[1024];

            /* int sd = socket(PF_INET, SOCK_RAW, proto->p_proto); */
            int sd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
            assert (sd >= 0);

            struct sockaddr_in addr;
            int len = sizeof(addr);
            int bytes = recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr*)&addr, (socklen_t*)&len);
            assert (bytes > 0);
            display(buf, bytes);
        }
};

uint16_t IcmpHeader::sequenceIndex = 1;

int main()
{
    assert (sizeof(IcmpHeader) == 1 + 1 + 2 + 2 + 2 + 8);

    struct hostent *hname = gethostbyname("127.0.0.1");
    struct sockaddr_in addr;
    /* bzero(&addr, sizeof(addr)); */
    addr.sin_family = hname->h_addrtype;
    addr.sin_port = 0;
    addr.sin_addr.s_addr = *(long*)hname->h_addr;

    IcmpHeader icmpHeader;
    icmpHeader.request(&addr);
    icmpHeader.response();
}
