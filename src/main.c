/*******************************************************************************
  SAME70 main.c  (SPI0 master -> read from Mega2560, then USART1 out)
*******************************************************************************/

#include "definitions.h"    // Harmony system/PLIB prototypes
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "usr/print_fuction.h"
#include "usr/rb.h"
#include "usr/delay.h"
#include "usr/spi_common.h"

/* ===== User Config ===== */
#define SPI_READ_CHUNK          512            
#define RB_SIZE                 8192      
/* ======================= */


/* -------- Globals -------- */
static uint8_t  rb_storage[RB_SIZE];
static RingBuffer rxRB;

int main(void)
{
    SYS_Initialize(NULL);
    rb_init_static(&rxRB, rb_storage, sizeof(rb_storage));
    USART1_WriteString("\n@SAME70 SPI->UART with rb.c\r");
    delay_init(300000000);

    const uint8_t tx[] = { 0xDE, 0xAD, 0xBE, 0xEF };
    uint8_t rx[sizeof(tx)];

    while (1) {
        delay_ms(3000);
        USART1_WriteString("\ndelay 3s\r\n");

//        memset(rx, 0, sizeof(rx));
//        if (SPI0_BlockingFullDuplex(tx, rx, sizeof(tx))) {
//            USART1_WriteString("FD OK, RX: ");
//            USART1_WriteHexDump(rx, sizeof(rx));
//            while (USART1_WriteIsBusy()) { }   // Flush
//            USART1_WriteString("\r\n");
//        } else {
//            USART1_WriteString("FD FAIL\r\n");
//        }

        // ? ?? ????: ????? ??? ?? ??? ????? '??'
        memset(rx, 0, sizeof(rx));
        if (SPI0_ReadBlocking(rx, sizeof(rx))) {     // << ?? TX? ?? ??
            USART1_WriteString("RD OK, RX: ");
            USART1_WriteHexDump(rx, sizeof(rx));
            while (USART1_WriteIsBusy()) { }   // Flush
            USART1_WriteString("\r\n");
        } else {
            USART1_WriteString("RD FAIL\r\n");
        }
    }
}