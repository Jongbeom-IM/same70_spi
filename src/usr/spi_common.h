#ifndef SPI_COMMON_H
#define SPI_COMMON_H

#include <stdint.h>
#include <stddef.h>    
#include <stdbool.h>    
#include <string.h>
#include "definitions.h"  

#if USE_GPIO_CS
#define CS_LOW()   SPI0_SS0_Clear()
#define CS_HIGH()  SPI0_SS0_Set()
#endif

bool SPI0_WriteBlocking(const uint8_t *data, size_t len);
bool SPI0_BlockingFullDuplex(const uint8_t* src, uint8_t* dst, size_t n);
bool SPI0_ReadBlocking(uint8_t* dst, size_t n);

#endif /* SPI_COMMON_H */
