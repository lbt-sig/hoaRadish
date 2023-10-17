#include "HCrc.h"

uint16_t calccrc(uint8_t crcbuf, uint16_t crc)
{
    uint8_t i;

    crc = crc ^ crcbuf;

    for (i = 0; i < 8; i++)
    {
        uint8_t chk;

        chk = crc & 1;

        crc = crc >> 1;

        crc = crc & 0x7fff;

        if (chk == 1)
            crc = crc ^ 0xa001;

        crc = crc & 0xffff;
    }

    return crc;
}

uint16_t chkcrc(uint8_t *buf, uint8_t len)
{
    uint8_t hi, lo;

    uint8_t i;

    uint16_t crc;

    crc = 0xFFFF;

    for (i = 0; i < len; i++)
    {
        crc = calccrc(*buf, crc);

        buf++;
    }

    hi = static_cast<uint8_t>(crc % 256);

    lo = crc / 256;

    crc = static_cast<uint16_t>((hi << 8) | lo);

    return crc;
}
