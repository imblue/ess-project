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

uint16_t getADC(uint8_t value1, uint8_t value2, SPI_Handle spi);

int output_UART_read(UART_Handle uart) {

    char lb[2] = "\r\n";

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
            return 0;
        } else {
            // TODO Unknown command
        }
    }

    return 0;
}

int input_UART_read(UART_Handle uart) {
	
	uint16_t XValue;
    uint16_t YValue;
	uint8_t ui8button;
	
	XValue = getADC(6, 64, spi);
    YValue = getADC(6, 0, spi);

    System_printf("Wert X-Achse: %d   \tWert Y-Achse: %d\n", XValue, YValue);
    System_flush();
	
	if (ui8button == 0)
    {
        Task_sleep(10);
        System_printf("CLICK\n");
        System_flush();
    }
    else {};
}