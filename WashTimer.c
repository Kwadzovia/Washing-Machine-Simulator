/*
 * WashTimer.c
 *
 *  Created on: Mar 14, 2020
 *      Author: selik
 */
// Counts down from 9 to 0 on 7-seg

#include <stdint.h>
#include "tm4c123gh6pm.h"

volatile extern unsigned long washCount;
volatile extern unsigned long systickCount;

void Wash_Timer(void){
    // Initialize count and timer
    washCount = 0x90;
    systickCount = 1000;

    while(washCount > 0)
    {
        GPIO_PORTB_DATA_R = washCount;

    }
    GPIO_PORTB_DATA_R = 0x00;
}



