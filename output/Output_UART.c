/*
 * Output_UART.c
 *
 *  Created on: 16.01.2019
 *      Author: Christian
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
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

    while (1) {
        char input[2];
        UART_read(uart, &input, 2);

        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 1);
        UART_write(uart, &input, 2); // Echo

        Task_sleep(5);
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);

        uint8_t selectedLed;
        sscanf(input, "%d", &selectedLed);

        uint32_t leds = 0;
        leds |= 1 << (selectedLed - 1);

        show_leds(leds);
    }

    return 0;
}

