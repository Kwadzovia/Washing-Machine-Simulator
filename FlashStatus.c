/*
 * FlashStatus.c
 *
 *  Created on: Mar 17, 2020
 *      Author: Caleb
 */
// Takes the current cycle status and flashes it briefly, finishing
// with the status displayed

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FlashStatus.h"

void FlashStatus(unsigned long status) {

    flash_count = 2;
    systickCount = 1000;
    while (flash_count > 0) {
        if (flash_status == TRUE) {
            GPIO_PORTB_DATA_R = status;
        } else {
            GPIO_PORTB_DATA_R = BLANK;
        }
    }
    GPIO_PORTB_DATA_R = status;


}
