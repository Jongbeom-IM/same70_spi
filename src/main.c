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
    SPI0_REGS->SPI_MR &= ~SPI_MR_LLB(1);
    rb_init_static(&rxRB, rb_storage, sizeof(rb_storage));
    USART1_WriteString("\n@SAME70 SPI->UART with rb.c\r");
    delay_init(300000000);
    
//    const uint8_t tx[] = { 0xDE, 0xAD, 0xBE, 0xEF };
    uint8_t rx[257];

    memset(rx, 0x0, sizeof(rx));
    if (SPI0_Read((void*)rx, sizeof(rx))) {  
        while (USART1_WriteIsBusy()) { }
        USART1_Write((void*)rx, sizeof(rx));  
        while (USART1_WriteIsBusy()) { }
    } else {
        USART1_WriteString("RD FAIL\r\n");
    }
}