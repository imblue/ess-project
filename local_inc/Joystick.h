#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <ti/drivers/SPI.h>

void InputFxn(UArg arg0, UArg arg1);
int setup_Task(SPI_Handle spi);
void setup_input_task(void);
uint16_t getADC(uint8_t value1, uint8_t value2, SPI_Handle spi);

#endif
