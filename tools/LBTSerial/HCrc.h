#ifndef HCRC_H
#define HCRC_H

#include <stdint.h>

// 计算crc.
uint16_t calccrc(uint8_t crcbuf, uint16_t crc);

// 检查crc
uint16_t chkcrc(uint8_t *buf, uint8_t len);

#endif // HCRC_H
