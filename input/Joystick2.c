/*
 * Alexander Feldinger, ic17b055
 * Version: 1.1
 * *
 *  ======== Joystick2.c ========
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

/* Driverlib headers */
#include <driverlib/gpio.h>
#include <ti/drivers/gpio/GPIOTiva.h>
#include <driverlib/rom.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/drivers/UART.h>

/* Board Header files */
#include <Board.h>
#include <EK_TM4C1294XL.h>
#include <driverlib/pin_map.h>/*supplies GPIO_PIN_x*/
#include <Joystick.h>
#include <driverlib/sysctl.h>

/* Defines */
#define CSON GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_5, 0);
#define CSOFF GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_5, 1);

int main(void)
{
    Board_initGeneral(120 * 1000 * 1000);

    GPIOPinTypeGPIOOutput(GPIO_PORTQ_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); //SPI
    GPIOPinTypeGPIOOutput(GPIO_PORTP_BASE, GPIO_PIN_5); //CS
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_0); //Button

    Board_initSPI();
    Board_initUART();

    UInt peripheralNum = 1;
    SPI_Handle spi;
    SPI_Params spiParams;

    SPI_Params_init(&spiParams);
    spiParams.mode = SPI_MASTER;
    spiParams.transferMode = SPI_MODE_BLOCKING;
    spiParams.transferCallbackFxn = NULL;
    spiParams.frameFormat = SPI_POL0_PHA0; //Clock Polarity, Clock Phase;
    spiParams.bitRate = 1000000; //HZ
    spiParams.dataSize = 8;

    spi = SPI_open(peripheralNum, &spiParams);

    if (spi == NULL){
        System_printf("Error opening SPI\n");
        }
        else{
        System_printf("SPI-Handle: %p\n", spi);
        }

    (void)setup_Task(spi);
    (void)setup_TaskUart;

    System_printf("Start BIOS\n");
    System_flush();

    BIOS_start();
}

