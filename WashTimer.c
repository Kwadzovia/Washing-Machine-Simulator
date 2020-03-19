/*
 * WashTimer.c
 *
 *  Created on: Mar 14, 2020
 *      Author: selik
 */
// Counts down from the number passed to 0 on 7-seg

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "MyDefines.h"

volatile extern unsigned long washCount;
volatile extern unsigned long systickCount;

// TODO: Turn this into a timer. Rather than just starting at some number and counting down,
// we could make this like a little spinner. It starts at the bottom  and goes around cw until it completes the circle.
// This would make a better representation of a timer. Not sure it's possible to do with the 74LS47 chip.
void WashTimer(unsigned long startCount){
    // Initialize count and timer
    washCount = startCount;
    systickCount = SYS_TICK_MAX;

    while(washCount > 0)
    {
        GPIO_PORTB_DATA_R = washCount;

    }
    GPIO_PORTB_DATA_R = 0x00;
}



