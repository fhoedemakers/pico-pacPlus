#ifndef __CRC32_H
#define __CRC32_H
#include <stdint.h>

uint32_t crc32_buf(const void *buf, long len);
uint32_t crc32_file(const char *filename);

#endif
