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
#include "ProgramSelect.h"

void ResetSwitches(void);

volatile extern unsigned long program;

// TODO: Add error when 110 or 111 are selected as they do not correlate to a program selection.
unsigned long Program_Select(){

    // Need this to when we loop around or it will immediately start again
    ResetSwitches();

    while(1)
    {
        //Accept Button
        if((GPIO_PORTA_DATA_R & ACCEPT_BUTTON) == ACCEPT_BUTTON)
        {
            program = (GPIO_PORTA_DATA_R & 0XE0) >> 5; // Only saves pins A7 - A5 then shifts them to the right to start at LSB (00000###)
            return program;
        }
        // Reset Button
        else if ((GPIO_PORTA_DATA_R & RESET_BUTTON) == RESET_BUTTON)
        {
            ResetSwitches();
        }
    }
}


