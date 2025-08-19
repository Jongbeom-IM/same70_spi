#include "definitions.h"
#include <stdint.h>
#include <stddef.h>   // size_t
#include <string.h>   // strlen

static inline void USART1_WriteBlock(const void* buf, size_t len)
{
    while (USART1_WriteIsBusy()) {}
    USART1_Write((void*)buf, len);
}

void USART1_WriteString(const char *s)
{
    USART1_WriteBlock(s, strlen(s));
}

void USART1_WriteHex8(uint8_t v)
{
    static const char hex[] = "0123456789ABCDEF";
    char out[4];
    out[0] = hex[(v >> 4) & 0xF];
    out[1] = hex[v & 0xF];
    out[2] = '\r';
    out[3] = '\n';
    USART1_WriteBlock(out, sizeof out);
}

void USART1_WriteHexDump(const uint8_t *p, size_t n)
{
    static const char hex[] = "0123456789ABCDEF";
    char line[3*16 + 2];              // "XX " * 16 + "\r\n"
    while (n) {
        size_t chunk = (n > 16) ? 16 : n;
        size_t pos = 0;
        for (size_t i = 0; i < chunk; ++i) {
            uint8_t b = *p++;
            line[pos++] = hex[(b >> 4) & 0xF];
            line[pos++] = hex[b & 0xF];
            line[pos++] = ' ';
        }
        line[pos++] = '\r';
        line[pos++] = '\n';
        USART1_WriteBlock(line, pos);
        n -= chunk;
    }
}

void USART1_WriteNumber(uint8_t num)
{
    char buf[5];
    int len = 0;
    if (num >= 100) { buf[len++] = '0' + (num / 100); num %= 100; }
    if (len || num >= 10) { buf[len++] = '0' + (num / 10); num %= 10; }
    buf[len++] = '0' + num;
    buf[len++] = '\r';
    buf[len++] = '\n';
    USART1_WriteBlock(buf, (size_t)len);
}