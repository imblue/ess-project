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
#include "InputOutput_Connector.h"

UART_Handle uart;

int output_UART_read(UART_Handle _uart) {
    uart = _uart;

    char lb[2] = "\r\n";

    setDebugMode(1);

    while (1) {
        // Mode
        char inputMode;
        UART_read(uart, &inputMode, 1);

        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 1);
        UART_write(uart, &inputMode, 1); // Echo
        UART_write(uart, &lb, 2); // Echo
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);

        if (inputMode == 'l') {
            char inputL[2];
            UART_read(uart, &inputL, 2);

            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 1);
            UART_write(uart, &inputL, 2); // Echo
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);

            uint8_t selectedLed;
            sscanf(inputL, "%d", &selectedLed);

            uint32_t leds = 0;
            leds |= 1 << (selectedLed - 1);

            show_leds(leds);
        } else if (inputMode == 'b') {

            char inputB;
            UART_read(uart, &inputB, 1);

            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 1);
            UART_write(uart, &inputB, 1); // Echo
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);

            if (inputB == '1') {
                changePressedState(1);
            } else {
                changePressedState(0);
            }
        } else if (inputMode == 'p') {
            uint8_t inputP[3];
            UART_read(uart, &inputP, 3);

            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 1);
            UART_write(uart, &inputP, 3); // Echo
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);

            uint8_t selectedPwm;
            sscanf(inputP, "%d", &selectedPwm);
            set_pwm(selectedPwm);
        } else if (inputMode == 'e') {
            setDebugMode(0);
            return 0;
        } else {
            // TODO Unknown command
        }
    }

    return 0;
}

void output_send(char msg[]) {
    if (uart == NULL) {
        return;
    }

    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 1);
    UART_write(uart, &msg, sizeof(msg));
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
}
