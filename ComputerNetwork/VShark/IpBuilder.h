#ifndef IP_BUILDER_H
#define IP_BUILDER_H

#include "parse.h"
#include <cstring>

class Fragment
{
    public:
        uint16_t length;
        IpHeader *ipHeader;

    public:
        Fragment(IpHeader *buffer, int length)
            : length(length)
        {
            ipHeader = (IpHeader *)(new uint8_t[length]);
            std::memcpy(ipHeader, buffer, length);
        }

        ~Fragment()
        {
            delete[] ipHeader;
        }
};

class IpBuilder()
{
    private:
        typedef uint16_t Offset;
        typedef uint16_t Identifier;
        typedef std::map<Offset, Fragment> FragmentsOfDatagram;
        typedef std::map<Identifier, std::pair<Offset, FragmentsOfDatagram>> Fragments;

    private:
        Fragments fragments;

    public:
        void addFragment(IpHeader *ipHeader, int length)
        {
            uint16_t identifier = ipHeader->identifier;
            Fragments::const_iterator itFragmentsOfDatagram = fragments.find(identifier);
            if (itFragmentsOfDatagram == fragments.end())
            {
                fragments[identifer] = std::pair(0, FragmentsOfDatagram());
                itFragmentsOfDatagram = fragments.find(identifier);
                assert (itFragmentsOfDatagram != ipHeaders.end());
            }

            uint16_t offset = ipHeader->fragmentationOffset;
            FragmentsOfDatagram::iterator itFragment = itFragmentsOfDatagram->second->find(offset);
            if (itFragment == itFragmentsDatagram->second->end())
            {
                /* 收到重复的Fragments，直接丢掉 */
                return;
            }
            (*itFragmentsOfDatagram)[offset] = Fragment(ipHeader, length);

            /* 更新期望收到的Fragments的Offset */
            if (itFragmentsOfDatagram->first == offset)
            {
                itFragmentsOfDatagram->first += (length - sizeof(IpHeader));
            }
            while (true)
            {
                FragmentsOfDatagram::iterator itFragment = itFragmentsOfDatagram->second->find(itFragmentsOfDatagram->first);
                if (itFragment == itFragmentsOfDatagram->second->end())
                {
                    break;
                }
                else
                {
                    IpHeader *ipHeader = itFragment->ipHeader;
                    Offset length = ipHeader->length;
                    itFragmentsOfDatagram->first += length;
                }
            }
        }
};

#endif
