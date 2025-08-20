#ifndef USART1_CUSTOM_H
#define USART1_CUSTOM_H

#include <stddef.h>
#include <stdint.h>

void USART1_WriteBlock(const void* buf, size_t len);
void USART1_WriteString(const char *s);
void USART1_WriteHex8(uint8_t v);
void USART1_WriteHexDump(const uint8_t *p, size_t n);
void USART1_WriteNumber(uint8_t num);

#endif
