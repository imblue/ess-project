/*
 * Output_UART.c
 *
 *  Created on: 16.01.2019
 *      Author: Christian
 */

#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <driverlib/sysctl.h>
#include <ti/drivers/UART.h>

/* Driverlib headers */
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>

/*Board Header files */
#include <Board.h>
#include <EK_TM4C1294XL.h>

/* Application headers */
#include "Output_Task.h"
#include "Output_UART.h"

int output_UART_read(UART_Handle uart) {
    int leds = 0;

    int i;
    for (i = 0; i < 8; i++) {
        char input;
        UART_read(uart, &input, 1);
        if (input == '1') {
            leds |= 1 << i;
        }

        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 1);
        UART_write(uart, &input, 1); // Remove this line to stop echoing!

        Task_sleep(5);
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
    }

    show_leds(leds);

    return 0;
}

