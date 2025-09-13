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
#define RX_CHUNK                64
/* ======================= */


/* -------- Globals -------- */
static uint8_t  rb_storage[RB_SIZE];
static RingBuffer rxRB;
typedef struct{
    volatile bool rxDone;
    uint8_t       rxBuf[RX_CHUNK];
}us_port_t;

static us_port_t g_u1 = {0};
static us_port_t g_u2 = {0};
static us_port_t g_u3 = {0};
static us_port_t g_u4 = {0};
static us_port_t g_us0 = {0};
static us_port_t g_us2 = {0};

/* ===== Callback ===== */
static void US_RxCallback(uintptr_t context){
    us_port_t* usp = (us_port_t*)context;
    usp->rxDone = true;
}


int main(void)
{
    SYS_Initialize(NULL);
    SPI0_REGS->SPI_MR &= ~SPI_MR_LLB(1);
    rb_init_static(&rxRB, rb_storage, sizeof(rb_storage));
    
    UART1_ReadCallbackRegister(US_RxCallback, (uintptr_t)&g_u1);
    bool u_ok1 = UART1_Read(g_u1.rxBuf, RX_CHUNK);   
    (void)u_ok1; 
    UART2_ReadCallbackRegister(US_RxCallback, (uintptr_t)&g_u2);
    bool u_ok2 = UART1_Read(g_u1.rxBuf, RX_CHUNK);   
    (void)u_ok2; 
    UART3_ReadCallbackRegister(US_RxCallback, (uintptr_t)&g_u3);
    bool u_ok3 = UART1_Read(g_u1.rxBuf, RX_CHUNK);   
    (void)u_ok3; 
    UART4_ReadCallbackRegister(US_RxCallback, (uintptr_t)&g_u4);
    bool u_ok4 = UART1_Read(g_u1.rxBuf, RX_CHUNK);   
    (void)u_ok4;
    USART0_ReadCallbackRegister(US_RxCallback, (uintptr_t)&g_us0);
    bool us_ok0 = UART1_Read(g_us0.rxBuf, RX_CHUNK);   
    (void)us_ok0; // false? ?? busy ? ?? ? ???
    USART2_ReadCallbackRegister(US_RxCallback, (uintptr_t)&g_us2);
    bool us_ok2 = UART1_Read(g_us2.rxBuf, RX_CHUNK);   
    (void)us_ok2; 
    
    USART1_WriteString("\n@SAME70 SPI->UART with rb.c\r");
    delay_init(300000000);
    
//    const uint8_t tx[] = { 0xDE, 0xAD, 0xBE, 0xEF };
    uint8_t rx[256];

    memset(rx, 0x0, sizeof(rx));
    if (SPI0_Read((void*)rx, sizeof(rx))) {  
        while (USART1_WriteIsBusy()) { }
        USART1_Write((void*)rx, sizeof(rx));  
        while (USART1_WriteIsBusy()) { }
    } else {
        USART1_WriteString("RD FAIL\r\n");
    }
}