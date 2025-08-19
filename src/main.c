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
//static uint8_t  spiChunk[SPI_READ_CHUNK];

int main(void){
    SYS_Initialize(NULL);
    rb_init_static(&rxRB, rb_storage, sizeof(rb_storage));
    USART1_WriteString("\n@SAME70 SPI->UART with rb.c\r");
    delay_init(300000000);
    
    while (1) {
        delay_ms(1000);
        USART1_WriteString("\ndelay 1s\r"); 
        const uint8_t tx[] = { 0xDE, 0xAD, 0xBE, 0xEF };
        if (SPI0_WriteBlocking(tx, sizeof(tx))) {
            USART1_WriteString("\nSPI0 write OK\r");
        } 
        else {
            USART1_WriteString("\nSPI0 write FAILED\r");
        }

    }
}