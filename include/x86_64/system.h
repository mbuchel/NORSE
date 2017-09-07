#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

void outputb(const uint16_t port, const uint8_t data);

uint8_t inportb(const uint16_t port);

#endif
