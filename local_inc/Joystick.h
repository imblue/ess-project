#ifndef JOYSTICK_H_
#define JOYSTICK_H_

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

/* Currently unused RTOS headers that are needed
 * for advanced features like IPC. */
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/hal/Timer.h>
#include <ti/drivers/SPI.h>

/* Driverlib headers */
#include <driverlib/gpio.h>
#include <ti/drivers/gpio/GPIOTiva.h>
#include <driverlib/rom.h>

/* Board Header files */
#include <Board.h>
#include <EK_TM4C1294XL.h>
#include <driverlib/pin_map.h>/*supplies GPIO_PIN_x*/


void OutputFxn(UArg arg0, UArg arg1);
int setup_Task(SPI_Handle spi);

#endif
