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

static inline char hex_nib(uint8_t v) { 
    return (v < 10) ? ('0' + v) : ('A' + (v - 10)); 
}

void USART1_WriteHexDump(const uint8_t* p, size_t n)
{
    char out[3];
    for (size_t i = 0; i < n; i++) {
        static const char H[] = "0123456789ABCDEF";
        out[0] = H[p[i] >> 4];
        out[1] = H[p[i] & 0x0F];
        out[2] = ' ';
        while (USART1_WriteIsBusy()) {}
        USART1_Write(out, 3);     
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

void USART1_WriteDecDump(const uint8_t* p, size_t n){
    char buf[5];
    for (size_t i = 0; i < n; i++) {
        int len = 0;
        if (p[i] >= 100) { 
            buf[len++] = '0' + (p[i] / 100); 
            p[i] %= 100; 
        }
        if (len || p[i] >= 10) { 
            buf[len++] = '0' + (p[i] / 10); 
            p[i] %= 10; 
        }
        buf[len++] = '0' + p[i];
        buf[len++] = '\r';
        buf[len++] = '\n';
        USART1_WriteBlock(buf, (size_t)len);
    }
}