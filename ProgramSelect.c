/*
 * ProgramSelect.c
 *
 *  Created on: Mar 12, 2020
 *      Author: selik
 */

// This is the first program to be run, pressing the switch triggers an interrupt incrementing the counter.
// Incrementing the counter

#include <stdint.h>
#include "tm4c123gh6pm.h"


volatile extern unsigned long menuCount;
volatile unsigned long menuTemp;
volatile extern unsigned long accept_flag;


void Program_Select(){

    while(1)
    {
        menuTemp = menuCount << 1;
        GPIO_PORTF_DATA_R &= 0xF1;
        GPIO_PORTF_DATA_R |= menuTemp;  // Cycle Ports 1:3 depending on count value

        if(accept_flag != 0) // Program Selected
        {
            return;
        }


    }

}


