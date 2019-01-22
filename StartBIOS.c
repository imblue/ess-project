/*
 *  ======== StartBIOS.c ========
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

/* Driverlib headers */
#include <driverlib/gpio.h>

/* Board Header files */
#include <Board.h>
#include <EK_TM4C1294XL.h>

/* Application headers */
#include <Output_Task.h>
#include <UART_Task.h>
#include <Joystick.h>

int main(void) {
    uint32_t ui32SysClock;
    ui32SysClock = Board_initGeneral(120*1000*1000);
	
	Board_initSPI();
	
    // TODO Input-Task
	setup_input_task();
	
    // Setup Output-Task
    setup_spi();
    (void) setup_Output_Task(15, "OutputTask 1", 500);
    System_printf("Created Output Task\n");

    // Setup UART
    (void)setup_UART_Task();
    System_printf("Created UART Task\n");

    System_printf("Starting BIOS\n");
    System_flush();

    // Start BIOS
    BIOS_start();
}
