#include <arpa/inet.h>
#include <iostream>
#include <iomanip>

namespace include
{
    namespace wrapper
    {
        inline uint16_t __htons__(uint16_t bytes)
        {
            return htons(bytes);
        }

        inline uint32_t __htonl__(uint32_t bytes)
        {
            return htonl(bytes);
        }

        inline uint16_t __ntohs__(uint16_t bytes)
        {
            return ntohs(bytes);
        }

        inline uint32_t __ntohl__(uint32_t bytes)
        {
            return ntohl(bytes);
        }

        #undef htons
        #undef htonl
        #undef ntohs
        #undef ntohl
    }

    inline uint16_t htons(uint16_t bytes)
    {
        return wrapper::__htons__(bytes);
    }

    inline uint32_t htonl(uint32_t bytes)
    {
        return wrapper::__htonl__(bytes);
    }

    inline uint16_t ntohs(uint16_t bytes)
    {
        return wrapper::__ntohs__(bytes);
    }

    inline uint32_t ntohl(uint32_t bytes)
    {
        return wrapper::__ntohl__(bytes);
    }
}

namespace demons
{
    void printBytes(uint32_t *bytes, int len)
    {
        for (int i = 0; i < len; i++)
        {
            std::cout << std::hex << std::setw(8) << std::setfill('0') << bytes[i] << ' ';
        }
        std::cout << std::endl;
    }

    uint32_t helper(uint32_t bytes, uint16_t (*pf)(uint16_t), bool exchangeLowHigh)
    {
        uint16_t high = bytes >> 16;
        uint16_t low = bytes & 0x0000ffff;
    
        high = (*pf)(high);
        low = (*pf)(low);
    
        if (exchangeLowHigh)
        {
            bytes = low;
            bytes = (bytes << 16) | high;
        }
        else
        {
            bytes = high;
            bytes = (bytes << 16) | low;
        }
    
        return bytes;
    }
    
    uint32_t htonl(uint32_t bytes)
    {
        return helper(bytes, ::include::htons, true);
    }

    uint32_t htonsWrapper(uint32_t bytes)
    {
        return helper(bytes, ::include::htons, false);
    }
    
    uint32_t ntohl(uint32_t bytes)
    {
        return helper(bytes, ::include::ntohs, true);
    }

    uint32_t ntohsWrapper(uint32_t bytes)
    {
        return helper(bytes, ::include::ntohs, false);
    }
    
    void convert(uint32_t* src, uint32_t* dst, int len, uint32_t (*pf)(uint32_t), bool print = false)
    {
        for (int i = 0; i < len; i++)
        {
            dst[i] = (*pf)(src[i]);
        }
        if (print)
        {
            printBytes(dst, len);
        }
    }
}

int main()
{
    int len = 10;
    uint32_t hostBytes[len];
    uint32_t networkBytes[len];
    
    for (int i = 0; i < len; i++)
    {
        hostBytes[i] = std::rand();
    }
    
    demons::convert(hostBytes, networkBytes, len, demons::htonsWrapper, true);
    demons::convert(hostBytes, networkBytes, len, include::htonl, true);
    demons::convert(hostBytes, networkBytes, len, demons::htonl, true);
    
    demons::convert(networkBytes, hostBytes, len, demons::ntohsWrapper, true);
    demons::convert(networkBytes, hostBytes, len, include::ntohl, true);
    demons::convert(networkBytes, hostBytes, len, demons::ntohl, true);
}
