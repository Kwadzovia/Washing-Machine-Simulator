/*
 * ProgramSelect.c
 *
 *  Created on: Mar 12, 2020
 *      Author: selik
 *
 *  Modified on: Mar 15, 2020
 *      Author: Caleb
 */

// This is the first program to be run. The program reads port A and depending on the value of
// menuCount we can determine the program that is selected

#include <stdint.h>
#include "tm4c123gh6pm.h"

void ResetSwitches(void);

volatile extern unsigned long menuCount;
volatile unsigned long menuTemp;


unsigned long Program_Select(){

    while(1)
    {
        //Accept Button
        if((GPIO_PORTA_DATA_R & 0x08) == 0x08)
        {
            menuCount = GPIO_PORTF_DATA_R;
            return menuCount;
        }
        // Reset Button
        else if ((GPIO_PORTA_DATA_R & 0x04) == 0x04)
        {
            ResetSwitches();
        }
    }
}


