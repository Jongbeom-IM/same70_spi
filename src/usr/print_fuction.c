#include "definitions.h"
#include <stdint.h>

static inline void usart1_write_str(const char *s) {
    while (*s) {
        USART1_Write((void*)s, 1);
        while (USART1_WriteIsBusy());
        s++;
    }
}

void USART1_WriteHex8(uint8_t v) {
    const char hex[] = "0123456789ABCDEF";
    char buf[5];
    buf[0] = hex[(v >> 4) & 0xF];
    buf[1] = hex[v & 0xF];
    buf[2] = '\r';
    buf[3] = '\n';
    buf[4] = '\0';
    usart1_write_str(buf);
}

void USART1_WriteNumber(uint8_t num) {
    char buf[5];
    int len = 0;
    if (num >= 100) {
        buf[len++] = '0' + (num / 100);
        num %= 100;
    }
    if (len > 0 || num >= 10) {
        buf[len++] = '0' + (num / 10);
        num %= 10;
    }
    buf[len++] = '0' + num;
    buf[len++] = '\r';
    buf[len++] = '\n';
    usart1_write_str(buf);
}

void USART1_WriteString(const char *s) {
    usart1_write_str(s);
}
