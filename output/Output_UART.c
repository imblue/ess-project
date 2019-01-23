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
#include "Joystick.h"

uint8_t activate;

void output_UART_write(UART_Handle uart, char msg[], Bool withLinebreak) {
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 1);
    UART_write(uart, &msg, sizeof(msg));

    if (withLinebreak) {
        char lb[2] = "\r\n";
        UART_write(uart, &lb, 2);
    }

    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
}

int output_UART_read(UART_Handle uart) {
    char info[] = "Usage:\r\nl => Set LED\r\nb => Simulate Button-Click\r\ne => Exit";
    output_UART_write(uart, info, 1);

    setDebugMode(1);

    while (1) {
        // Mode
        char inputModeS[1];
        UART_read(uart, &inputModeS, 1);

        output_UART_write(uart, inputModeS, 1);

        char inputMode = inputModeS[0];
        if (inputMode == 'l') {
            // TODO Info-Text

            char inputL[2];
            UART_read(uart, &inputL, 2);

            output_UART_write(uart, inputL, 1); // Echo

            int selectedLed;
            sscanf(inputL, "%d", &selectedLed);

            uint32_t leds = 0;
            leds |= 1 << (selectedLed - 1);

            show_leds(leds);
        } else if (inputMode == 'b') {
            // TODO Info-Text

            char inputB[1];
            UART_read(uart, &inputB, 1);

            output_UART_write(uart, inputB, 1); // Echo

            if (inputB[0] == '1') {
                changePressedState(1);
            } else {
                changePressedState(0);
            }
        } /*else if (inputMode == 'p') {
            uint8_t inputP[3];
            UART_read(uart, &inputP, 3);

            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 1);
            UART_write(uart, &inputP, 3); // Echo
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);

            uint8_t selectedPwm;
            sscanf(inputP, "%d", &selectedPwm);
            set_pwm(selectedPwm);
        }*/ else if (inputMode == 'e') {
            char exitInfo[] = "Exit Debug-Mode";
            output_UART_write(uart, exitInfo, 1);

            setDebugMode(0);
            changePressedState(0);
            return 0;
        } else { // Unknown command
            char ucInfo[] = "Unknown command";
            output_UART_write(uart, ucInfo, 1);
        }
    }
}

int input_UART_read(UART_Handle uart) {

    uint8_t ui8button;
    uint32_t ui32Strength;
    uint32_t ui32PinType;

    activate = 1;

    GPIOPadConfigGet(GPIO_PORTD_BASE, GPIO_PIN_4, &ui32Strength, &ui32PinType);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_4, ui32Strength, GPIO_PIN_TYPE_STD_WPU);
    ui8button = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_4);

    if (ui8button == 0)
    {
        System_printf("CLICK\n");
        System_flush();
    }
    else {};

    return 0;
}
