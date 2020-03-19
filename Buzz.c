/*
 * Buzz.c
 *
 *  Created on: Mar 19, 2020
 *      Author: selik
 */

//  Function just turns buzzer on or off
//  Timing must be done outside of function

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Buzz.h"

void Buzz(unsigned long state){

    if(state >= 1 )
    {

        GPIO_PORTF_DATA_R |= 0x04;  //Turn Buzzer On
    }
    else
    {
        GPIO_PORTF_DATA_R &= 0xFB;  //Turn Buzzer Off
    }

    return;
}

