/*
 * Alexander Feldinger, ic17b055
 * Version: 1.1
 *
 * ======== Joystick.c ========
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

/* Currently unused RTOS headers that are needed
 * for advanced features like IPC. */
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/hal/Timer.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>

/* Driverlib headers */
#include <driverlib/gpio.h>
#include <ti/drivers/gpio/GPIOTiva.h>
#include <driverlib/rom.h>

/* Board Header files */
#include <Board.h>
#include <EK_TM4C1294XL.h>
#include <driverlib/pin_map.h>/*supplies GPIO_PIN_x*/
#include "Joystick.h"
#include "InputOutput_Connector.h"

/* Defines */
#define CSON GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_5, 0);
#define CSOFF GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_5, GPIO_PIN_5);

uint8_t activate;

void InputFxn(UArg arg0, UArg arg1)
{
    SPI_Handle spi = (SPI_Handle)arg0;
    uint16_t XValue;
    uint16_t YValue;
    uint32_t ui32Strength;
    uint32_t ui32PinType;
    uint8_t ui8button;

    while(1)
    {
    XValue = getADC(6, 64, spi);
    YValue = getADC(6, 0, spi);

    GPIOPadConfigGet(GPIO_PORTD_BASE, GPIO_PIN_4, &ui32Strength, &ui32PinType);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_4, ui32Strength, GPIO_PIN_TYPE_STD_WPU);
    ui8button = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_4);

    if (activate){
        System_printf("Werte X-Achse: %d  \tWerte Y-Achse: %d\n", XValue, YValue);
        System_flush();
    }

    convert(XValue, YValue);

    if (ui8button == 0)
    {
        changePressedState(1);
        Task_sleep(10);
    }
    else {
        changePressedState(0);
    }

    Task_sleep(50);
    }
}

uint16_t getADC(uint8_t value1, uint8_t value2, SPI_Handle spi)
{
    uint16_t result;
    uint8_t Buffer1;
    uint8_t Buffer2;
    uint8_t Buffer3;
    uint8_t Byte1 = value1;
    uint8_t Byte2 = value2;

    SPI_Transaction spiTransaction;
    spiTransaction.count = 1;
    spiTransaction.txBuf = &Byte1;
    spiTransaction.rxBuf = &Buffer1;

    CSON
    SPI_transfer(spi, &spiTransaction);

    spiTransaction.txBuf = &Byte2;
    spiTransaction.rxBuf = &Buffer2;
    SPI_transfer(spi, &spiTransaction);

    spiTransaction.txBuf = NULL;
    spiTransaction.rxBuf = &Buffer3;
    SPI_transfer(spi, &spiTransaction);

    CSOFF

    result = Buffer2 & 0x0F;
    result = ((Uint16)result << 8) | Buffer3;
    return result;
}

int setup_Task(SPI_Handle spi)
{
    Task_Params taskParams;
    Task_Handle taskOutput;
    Error_Block eb;

    Error_init(&eb);
    Task_Params_init(&taskParams);
    taskParams.stackSize = 1064;
    taskParams.priority = 15;
    taskParams.arg0 = (UArg)spi;

    taskOutput = Task_create((Task_FuncPtr)InputFxn, &taskParams, &eb);
    if (taskOutput == NULL) {
       System_abort("Task creation failed");
       System_flush();

    }
    return (0);
}


