#ifndef SPI_COMMON_H
#define SPI_COMMON_H

#include <stdint.h>
#include <stddef.h>    
#include <stdbool.h>    
#include "definitions.h"  

#if USE_GPIO_CS
#define CS_LOW()   SPI0_SS0_Clear()
#define CS_HIGH()  SPI0_SS0_Set()
#endif

bool SPI0_WriteBlocking(const uint8_t *data, size_t len);

#endif /* SPI_COMMON_H */
