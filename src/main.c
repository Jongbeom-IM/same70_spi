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

/* ===== User Config ===== */
#define USE_GPIO_CS             0              
#define SPI_READ_CHUNK          512            
#define RB_SIZE                 8192           
#define MAX_DUMP_PER_LOOP       2048           
/* ======================= */

#if USE_GPIO_CS
#define CS_LOW()   SPI0_SS0_Clear()           
#define CS_HIGH()  SPI0_SS0_Set()
#endif

/* -------- Globals -------- */
static uint8_t  rb_storage[RB_SIZE];
static RingBuffer rxRB;
static uint8_t  spiChunk[SPI_READ_CHUNK];


void USART1_WriteCharBlocking(char c)
{
    while (USART1_WriteIsBusy()) {}
    USART1_Write(&c, 1);
    while (USART1_WriteIsBusy()) {}
}

void USART1_WriteStringBlocking(const char *s)
{
    while (*s) {
        USART1_WriteCharBlocking(*s++);
    }
}

int main(void){
    SYS_Initialize(NULL);
    rb_init_static(&rxRB, rb_storage, sizeof(rb_storage));
    USART1_WriteStringBlocking("\n@SAME70 SPI->UART with rb.c\r");
    delay_init(300000000);
    
    while (1) {
        delay_ms(1000);
        SPI0_Write()
//        if (SPI0_WriteRead(NULL, 0, spiChunk, sizeof(spiChunk))) {
//            while (SPI0_IsBusy()) {}
//            bool allFF = true;
//            for (size_t i = 0; i < sizeof(spiChunk); i++) {
//                if (spiChunk[i] != 0xFF) {
//                    allFF = false;
//                    break;
//                }
//            }
//            if (!allFF) {
//                rb_write_overwrite(&rxRB, spiChunk, sizeof(spiChunk));
//            }
//        }

//        size_t avail = rb_count(&rxRB);
//        if (avail) {
//            uint8_t out[MAX_DUMP_PER_LOOP];
//            size_t to_send = (avail > sizeof(out)) ? sizeof(out) : avail;
//            to_send &= ~(size_t)1;   
//            if (to_send) {
//                size_t got = rb_read(&rxRB, out, to_send);
//                if (got) {
//                    USART1_WriteHexDump(out, got);
//                    // while (USART1_WriteIsBusy()) {}
//                    // USART1_Write(out, got);
//                }
//            }
//        }
    }
}