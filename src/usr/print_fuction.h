#ifndef USART1_CUSTOM_H
#define USART1_CUSTOM_H

#include <stdint.h>

void USART1_WriteHex8(uint8_t v);
void USART1_WriteNumber(uint8_t num);
void USART1_WriteString(const char *s);

#endif
