/*******************************************************************************
  SAME70 main.c  (SPI0 master -> read from Mega2560, then USART1 out)

  - SPI0: 8bit, Mode 0, Baud ~1MHz (MHC?? ??)
  - USART1: 115200 ?? (MHC?? ??)
  - Mega ?? ?? 1?? ????? 2???? ?? ????? ??
*******************************************************************************/

#include "definitions.h"    // Harmony system/PLIB prototypes
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define RX_BYTES        4096           // ??? ??(2? ??)
#define TEXT_MODE       0              // 0:???? ???, 1:ASCII? ???

// -------- CS ?? ?? ?? --------
#define USE_GPIO_CS     0

#if USE_GPIO_CS
// ??? ?? ? ???? MHC? ??? ?? ? ???? ????.
// ?: #define CS_LOW()  SPI0_CS_Clear()
//     #define CS_HIGH() SPI0_CS_Set()
#define CS_LOW()   SPI0_SS0_Clear()    // TODO: ????? ?? ?? ??
#define CS_HIGH()  SPI0_SS0_Set()      // TODO: ????? ?? ?? ??
#endif

static uint8_t  rxBuf[RX_BYTES];

static inline void uart_write_block(const void* p, size_t n)
{
    while (USART1_WriteIsBusy()) {}
    USART1_Write((void*)p, n);
}

static inline void uart_write_str(const char* s)
{
    uart_write_block(s, strlen(s));
}

int main(void)
{
    SYS_Initialize(NULL);

    // ??? ??
    uart_write_str("@SAME70 SPI->UART bridge ready\r\n");

    while (true)
    {
#if USE_GPIO_CS
        CS_LOW();   // ??? ??
#endif
        // ?? ???? ?? ??? ??????? RX_BYTES ?? ??
        if (SPI0_WriteRead(NULL, 0, rxBuf, sizeof(rxBuf)))
        {
            while (SPI0_IsBusy()) { }
        }
#if USE_GPIO_CS
        CS_HIGH();  // ??? ??
#endif

#if TEXT_MODE
        // 16-bit ?????? ???? ASCII? ?? (???? ??? ???)
        // 4096??? ? 2048 ??
        char line[16];
        for (size_t i = 0; i < sizeof(rxBuf); i += 2)
        {
            uint16_t v = (uint16_t)rxBuf[i] | ((uint16_t)rxBuf[i+1] << 8);  // 0..1023
            int len = snprintf(line, sizeof(line), "%u\r\n", (unsigned)v);
            uart_write_block(line, (size_t)len);
        }
#else
        // ???? ?? ??? ?? (??) : ????? '<H'? ??
        uart_write_block(rxBuf, sizeof(rxBuf));
#endif
        // ?? ? ??? ? (??)
        // SYS_TIME_DelayMS(1);
    }

    // normally not reached
    // SYS_Tasks();
    return 0;
}
