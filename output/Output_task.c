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

/* Driverlib headers */
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>

/* Board Header files */
#include <Board.h>
#include <EK_TM4C1294XL.h>

/* Application headers */
#include "Output_Task.h"

/* SPI */
#include <ti/drivers/SPI.h>

// H2 top
/*#define LATCH_SYS_PORT SYSCTL_PERIPH_GPIOH
static const int LATCH_PORT = GPIO_PORTH_BASE;
static const int LATCH_PIN = GPIO_PIN_2;
#define PERIPHAL Board_SPI0*/

// P5 bottom
#define LATCH_SYS_PORT SYSCTL_PERIPH_GPIOP
static const int LATCH_PORT = GPIO_PORTP_BASE;
static const int LATCH_PIN = GPIO_PIN_5;
#define PERIPHAL Board_SPI1

SPI_Handle handle;

uint8_t leds = 0;

/*
 *  Perform blink operation on LED given as arg0, with period defined by arg1.
 */
void OutputFxn(UArg arg0, UArg arg1)
{
    uint32_t wait_ticks = (uint32_t) arg1;

    int i;
    for (i = 0; i < 8; i++) {
        leds |= 1 << i;
    }

    while (1) {

        Bool transferOK;
        SPI_Transaction spiTransaction;
        spiTransaction.count = 1;
        spiTransaction.txBuf = &leds;
        spiTransaction.rxBuf = NULL;

        transferOK = SPI_transfer(handle, &spiTransaction);
        // TODO for each
        transferOK = SPI_transfer(handle, &spiTransaction);
        transferOK = SPI_transfer(handle, &spiTransaction);
        transferOK = SPI_transfer(handle, &spiTransaction);

        if (!transferOK) {
            System_printf("Error in SPI transaction\n");
            System_flush();
        }
        else {
            System_printf("Successfull transaction\n");
            System_flush();
        }

        // Latch
        GPIOPinWrite(LATCH_PORT, LATCH_PIN, LATCH_PIN);
        GPIOPinWrite(LATCH_PORT, LATCH_PIN, 0);

        Task_sleep(wait_ticks);

        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_PIN_7);
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0);
    }
}


/*
 *  Setup task function
 */
int setup_Output_Task(int prio, xdc_String name, uint32_t wait_ticks) {
    Task_Handle handler;
    Task_Params taskParams;
    Error_Block eb;

    /* Create blink task with priority 15*/
    Error_init(&eb);
    Task_Params_init(&taskParams);
    taskParams.instance->name = name;
    taskParams.stackSize = 1024; /* stack in bytes */
    taskParams.priority = prio; /* 0-15 (15 is highest priority on default -> see RTOS Task configuration) */
    taskParams.arg0 = (UArg) name; // TODO Maybe something good can be sent later
    taskParams.arg1 = (UArg) wait_ticks; /* pass periods in ticks as arg1 */

    handler = Task_create((Task_FuncPtr) OutputFxn, &taskParams, &eb);
    if (handler == NULL) {
        System_abort("Error creating Output-Task");
    }

    return 0;
}

int setup_spi() {

    //Board_initGeneral(120 * 1000 * 1000);

    Board_initGPIO();
    Board_initSPI();

    // Reset = C7
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    //GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_7);

    // Latch = H2
    SysCtlPeripheralEnable(LATCH_SYS_PORT);
    GPIOPinTypeGPIOOutput(LATCH_PORT, LATCH_PIN);

    SPI_Params spiParams;

    SPI_Params_init(&spiParams);
    spiParams.transferMode = SPI_MODE_BLOCKING;
    spiParams.transferCallbackFxn = NULL;
    spiParams.bitRate = 2000000;
    spiParams.dataSize = 8;
    spiParams.frameFormat = SPI_POL0_PHA0;
    spiParams.mode = SPI_MASTER;

    handle = SPI_open(PERIPHAL, &spiParams);
    if (handle == NULL) {
      System_printf("SPI did not open\n");
    }

    System_flush();

    return 0;
}

int show_leds(uint8_t l) {
    leds = l;
    return 0;
}

int set_pwm(uint8_t pwm) {
    // TODO
    return 0;
}
