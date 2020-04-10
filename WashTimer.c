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



