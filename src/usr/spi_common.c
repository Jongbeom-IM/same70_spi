#include "spi_common.h"

bool SPI0_WriteBlocking(const uint8_t *data, size_t len)
{
#if USE_GPIO_CS
    CS_LOW();
#endif

    bool ok = SPI0_Write((void*)data, len);
    if (!ok) {
#if USE_GPIO_CS
        CS_HIGH();
#endif
        return false;
    }
    while (SPI0_IsBusy()) { }

#if USE_GPIO_CS
    CS_HIGH();
#endif
    return true;
}

bool SPI0_BlockingFullDuplex(const uint8_t* src, uint8_t* dst, size_t n)
{
#if USE_GPIO_CS
    CS_LOW();
#endif

    bool ok = SPI0_WriteRead((void*)src, n, (void*)dst, n);
    if (!ok) {
        return false;
    }
    
    while (SPI0_IsBusy()) { }
    
#if USE_GPIO_CS
    CS_HIGH();
#endif
    return true;
}

bool SPI0_ReadBlocking(uint8_t* dst, size_t n)
{
    // ?? ???? ?? ?? (0xFF ??)
    // n? ??? ??, ?? ??? ????.
    uint8_t dummy[64];
    bool ok = true;
    
#if USE_GPIO_CS
    CS_LOW();
#endif

    size_t rem = n;
    while (rem) {
        size_t chunk = (rem > sizeof(dummy)) ? sizeof(dummy) : rem;
        memset(dummy, 0xFF, chunk);

        bool started = SPI0_WriteRead(dummy, chunk, dst + (n - rem), chunk);
        if (!started) { ok = false; break; }

        while (SPI0_IsBusy()) { } // ??? ??
        rem -= chunk;
    }
    
#if USE_GPIO_CS
    CS_HIGH();
#endif
    return ok;
}