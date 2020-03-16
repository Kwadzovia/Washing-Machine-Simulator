/*
 * ResetSwitches.c
 *
 *  Created on: Mar 15, 2020
 *      Author: Caleb
 */

// This is to reset the switches back to their default state

#include <stdint.h>
#include "tm4c123gh6pm.h"


void ResetSwitches(){
    GPIO_PORTF_DATA_R = 0x00; // Reset switches
    GPIO_PORTF_DATA_R = 0x01; // Set it back to default
}


