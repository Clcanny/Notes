#include "helper.h"

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

    return (sum & 0xFFFF) == 0;
}
