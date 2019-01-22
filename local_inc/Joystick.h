#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>

void InputFxn(UArg arg0, UArg arg1);
int setup_Task(SPI_Handle spi);
int setup_input_task(void);

#endif
