/*
 * WashTimer.c
 *
 *  Created on: Mar 14, 2020
 *      Author: selik
 */
// Counts down from 5 on 7-seg

#include <stdint.h>
#include "tm4c123gh6pm.h"

volatile extern unsigned long washCount;
volatile extern unsigned long systickCount;

void Wash_Timer(void){
    //Pause timer
    //Set counter to 5
    //Reset timer
    //Start timer
    systickCount = 1000;
    washCount = 9;

    while(washCount > 0)
    {
        GPIO_PORTB_DATA_R &= 0xF0; // clear Port B
        GPIO_PORTB_DATA_R |= washCount;

    }
    GPIO_PORTB_DATA_R &= 0xF0; // clear Port B
    GPIO_PORTB_DATA_R |= washCount;

    return;
    //Loop if counter != 0
    //Display count on 7seg

    //Display 0

    //return


}



