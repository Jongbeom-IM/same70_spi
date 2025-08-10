/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "usr/print_fuction.h"


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main(void) {
    SYS_Initialize(NULL);

    uint8_t hansol[256];
    for (int i = 0; i < 256; i++) {
        hansol[i] = i;
    }
    USART1_WriteString("@Hello World!\r\n");

    for (int i = 0; i < 256; i++) {
        SPI0_Write(&hansol[i], sizeof(hansol[i]));
    }

    while (true) {
    }
}


/*******************************************************************************
 End of File
*/

