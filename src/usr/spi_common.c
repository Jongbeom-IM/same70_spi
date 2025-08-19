#include "spi_common.h"

bool SPI0_WriteBlocking(const uint8_t *data, size_t len)
{
#if USE_GPIO_CS
    CS_LOW();
#endif

    bool ok = SPI0_Write((void*)data, len);  // PLIB ??
    if (!ok) {
#if USE_GPIO_CS
        CS_HIGH();
#endif
        return false;
    }

    while (SPI0_IsBusy()) { }                // PLIB ??

#if USE_GPIO_CS
    CS_HIGH();
#endif
    return true;
}
