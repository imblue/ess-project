#ifndef OUTPUT_TASK_H_
#define OUTPUT_TASK_H_

#include <stdbool.h>
#include <stdint.h>
#include <xdc/std.h>

void OutputFxn(UArg arg0, UArg arg1);

int setup_Output_Task(int prio, xdc_String name, uint32_t wait_ticks);

int setup_spi();

int show_leds(uint32_t l);

int set_pwm(uint8_t pwm);

#endif
