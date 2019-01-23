/*
 * InputOutput_Connector.c
 *
 *  Created on: 22.01.2019
 *      Author: Christian
 */
#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <math.h>

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
#include "InputOutput_Connector.h"
#include "Output_Task.h"
#include "Output_UART.h"

Bool pressed = 0;
Bool debugMode = 0;

void convert(uint16_t x, uint16_t y) {
    // Do nothing if pressed or debug-mode enabled
    if (pressed || debugMode) {
        return;
    }

    Bool xMiddle = x > 2040 && x < 2050;
    Bool yMiddle = y > 2040 && y < 2050;

    uint32_t leds = 0;

    if (xMiddle && yMiddle) {
        show_leds(leds);
        return;
    }

    if (xMiddle) {
        if (y < 2046) { // Oben
            leds |= 1 << 8;
        } else { // Unten
            leds |= 1 << 24;
        }
        show_leds(leds);
        return;
    }

    if (yMiddle) {
        if (x < 2046) { // Links
            leds |= 1 << 16;
        } else { // Rechts
            leds |= 1 << 0;
        }
        show_leds(leds);
        return;
    }

    Bool rightSide = x > 2046;

    uint32_t a;
    // === Rechts
    if (rightSide) {
        a = x - 2046;
    } else {
        a = 2046 - x;
    }


    uint32_t b;
    if (y > 2046) {
        b = y - 2046;
    } else {
        b = 2046 - y;
    }

    double c = sqrt(pow(a, 2) + pow(b, 2));
    double ac = a / c;
    double alpha = asin(ac);

    double alphaProzent = alpha / 90 * 100;
    double led = 8 * alphaProzent;
    uint16_t ledint = led;

    // Damit eine leuchtet wenn 0,irgendwas
    if (ledint == 0) {
        ledint = 1;
    }

    if (rightSide) {
        if (y < 2046) { // Rechts oben
            leds |= 1 << (8 - ledint);
        } else { // Rechts unten
            leds |= 1 << (24 + ledint);
        }
    } else {
        if (y < 2046) { // Links oben
            leds |= 1 << (16 - ledint);
        } else { // Links unten
            leds |= 1 << (26 + ledint);
        }
    }

    show_leds(leds);
}

void changePressedState(Bool _pressed){
    if (pressed == _pressed) {
        return;
    }

    pressed = _pressed;

    uint32_t leds = 0;
    if (pressed) {
        int i;
        for (i = 0; i < 32; i++) {
            leds |= 1 << i;
        }
    }

    show_leds(leds);
    //set_pwm(100);
}

void setDebugMode(Bool _debugMode) {
    debugMode = _debugMode;
}

