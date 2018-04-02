#include "ping.h"

Ping::Ping()
{
    m_maxPacketSize = 4;
    m_datalen = 56;
    m_nsend = 0;
    m_nreceived = 0;
    m_icmp_seq = 0;
}

/*校验和算法*/
unsigned short Ping::getChksum(unsigned short *addr,int len)
{   
    int nleft = len;
    int sum = 0;
    unsigned short *w = addr;
    unsigned short answer = 0;

    while (nleft > 1)
    {
        sum += *w++;
        nleft -= 2;
    }
    if (nleft == 1)
    {
        *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return answer;
}

/* 设置ICMP报头 */
int Ping::packIcmp(int pack_no, struct icmp* icmp)
{   
    int i, packsize;
    struct icmp *picmp;
    struct timeval *tval;

    picmp = icmp;
    picmp->icmp_type = ICMP_ECHO;
    picmp->icmp_code = 0;
    picmp->icmp_cksum = 0;
    picmp->icmp_seq = pack_no;
    picmp->icmp_id = m_pid;
    packsize = 8 + m_datalen;
    tval = (struct timeval *)icmp->icmp_data;
    /* 记录发送时间 */
    gettimeofday(tval, NULL);
    picmp->icmp_cksum = getChksum((unsigned short *)icmp, packsize);
    return packsize;
}

/* 剥去ICMP报头 */
bool Ping::unpackIcmp(char *buf,int len, struct IcmpEchoReply *icmpEchoReply)
{   
    int i,iphdrlen;
    struct ip *ip;
    struct icmp *icmp;
    struct timeval *tvsend, tvrecv, tvresult;
    double rtt;

    ip = (struct ip *)buf;
    /* 求ip报头长度,即ip报头的长度标志乘4 */
    iphdrlen = ip->ip_hl << 2;
    /* 越过IP报头，指向ICMP报头 */
    icmp = (struct icmp *)(buf + iphdrlen);
    /* ICMP报头及ICMP数据报的总长度 */
    len -= iphdrlen;
    /* 小于ICMP报头长度则不合理 */
    if (len < 8)
    {
        printf("ICMP packets\'s length is less than 8\n");
        return false;
    }
    /* 确保所接收的是我所发的的ICMP的回应 */
    if ((icmp->icmp_type == ICMP_ECHOREPLY) && (icmp->icmp_id == m_pid))
    {
        tvsend = (struct timeval *)icmp->icmp_data;
        /* 记录接受时间 */
        gettimeofday(&tvrecv, NULL);
        /* 接受和发送的时间差 */
        tvresult = tvSub(tvrecv, *tvsend);
        /* 以毫秒为单位计算RTT */
        rtt=tvresult.tv_sec*1000 + tvresult.tv_usec/1000;
        icmpEchoReply->rtt = rtt;
        icmpEchoReply->icmpSeq = icmp->icmp_seq;
        icmpEchoReply->ipTtl = ip->ip_ttl;
        icmpEchoReply->icmpLen = len;
        return true;
    }
    else
    {
        return false;
    }
}

/* 两个timeval结构相减 */
struct timeval Ping::tvSub(struct timeval timeval1, struct timeval timeval2)
{
    struct timeval result;
    result = timeval1;
    if (result.tv_usec < timeval2.tv_usec < 0)
    {
        --result.tv_sec;
        result.tv_usec += 1000000;
    }
    result.tv_sec -= timeval2.tv_sec;
    return result;
}

/* 发送三个ICMP报文 */
bool Ping::sendPacket()
{   
    size_t packetsize;
    while (m_nsend < m_maxPacketSize)
    {
        m_nsend++;
        m_icmp_seq++;
        /* 设置ICMP报头 */
        packetsize = packIcmp(m_icmp_seq, (struct icmp*)m_sendpacket);

        if (sendto(m_sockfd, m_sendpacket, packetsize, 0, (struct sockaddr*)&m_dest_addr, sizeof(m_dest_addr)) < 0)
        {
            perror("sendto error");
            continue;
        }
    }
    return true;
}

/* 接收所有ICMP报文 */
bool Ping::recvPacket(PingResult &pingResult)
{
    int len;
    extern int errno;
    struct IcmpEchoReply icmpEchoReply;
    int maxfds = m_sockfd + 1;
    int nfd = 0;
    fd_set rset;
    FD_ZERO(&rset);
    socklen_t fromlen = sizeof(m_from_addr);
    struct timeval timeout;
    timeout.tv_sec = 4;
    timeout.tv_usec = 0;

    for (int recvCount = 0; recvCount < m_maxPacketSize; recvCount++)
    {
        FD_SET(m_sockfd, &rset);
        if ((nfd = select(maxfds, &rset, NULL, NULL, &timeout)) == -1)
        {
            perror("select error");
            continue;
        }
        if (nfd == 0)
        {
            /* recv time out */
            icmpEchoReply.isReply = false;
            pingResult.icmpEchoReplys.push_back(icmpEchoReply);
            continue;
        }
        if (FD_ISSET(m_sockfd, &rset))
        {
            if ((len = recvfrom(m_sockfd, m_recvpacket, sizeof(m_recvpacket), 0, (struct sockaddr *)&m_from_addr, &fromlen)) < 0)
            {
                if(errno == EINTR)
                {
                    continue;
                }
                perror("recvfrom error");
                continue;
            }
            icmpEchoReply.fromAddr = inet_ntoa(m_from_addr.sin_addr);
            if (icmpEchoReply.fromAddr != pingResult.ip) {
                /* retry again */
                recvCount--;
                continue;
            }
        }
        if (unpackIcmp(m_recvpacket, len, &icmpEchoReply)==-1)
        {
            /* retry again */
            recvCount--;
            continue;
        }
        icmpEchoReply.isReply = true;
        pingResult.icmpEchoReplys.push_back(icmpEchoReply);
        m_nreceived++;
    }
    return true;
}

bool Ping::getsockaddr(const char * hostOrIp, struct sockaddr_in* sockaddr)
{
    struct hostent *host;
    struct sockaddr_in dest_addr;
    unsigned long inaddr = 0l;
    bzero(&dest_addr, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    /* 判断是主机名还是ip地址 */
    if (inaddr = inet_addr(hostOrIp) == INADDR_NONE)
    {
        /* 主机名 */
        if ((host = gethostbyname(hostOrIp)) == NULL)
        {
            return false;
        }
        memcpy((char *)&dest_addr.sin_addr, host->h_addr, host->h_length);
    }
    /* IP地址 */
    else if (!inet_aton(hostOrIp, &dest_addr.sin_addr))
    {
        return false;
    }
    *sockaddr = dest_addr;
    return true;
}

bool Ping::ping(std::string host, PingResult& pingResult)
{
    return ping(host, 1, pingResult);
}

bool Ping::ping(std::string host, int count, PingResult& pingResult)
{
    int size = 50 * 1024;
    IcmpEchoReply icmpEchoReply;
    bool ret;

    m_nsend = 0;
    m_nreceived = 0;
    pingResult.icmpEchoReplys.clear();
    m_maxPacketSize = count;
    m_pid = getpid();

    pingResult.dataLen = m_datalen;

    /* 生成使用ICMP的原始套接字,这种套接字只有root才能生成 */
    if ((m_sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
    {
        extern int errno;
        pingResult.error = strerror(errno);
        return false;
    }
    /* 扩大套接字接收缓冲区到50K
     * 这样做主要为了减小接收缓冲区溢出的的可能性
     * 若无意中ping一个广播地址或多播地址
     * 将会引来大量应答
     */
    setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

    /* 获取main的进程id,用于设置ICMP的标志符 */
    if (!getsockaddr(host.c_str(), &m_dest_addr))
    {
        pingResult.error = "unknow host " + host;
        return false;
    }
    pingResult.ip = inet_ntoa(m_dest_addr.sin_addr);
    /* 发送所有ICMP报文 */
    sendPacket();
    /* 接收所有ICMP报文 */
    recvPacket(pingResult);
    pingResult.nsend = m_nsend;
    pingResult.nreceived = m_nreceived;
    close(m_sockfd);
    return true;
}
