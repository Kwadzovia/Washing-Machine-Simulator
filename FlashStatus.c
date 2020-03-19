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

    flash_count = FLASH_COUNT_MAX;
    flashtickCount = FLASH_TICK_MAX;
    flash_status = FALSE;

    while (flash_count > 0) {
        if (flash_status == TRUE) {
            GPIO_PORTB_DATA_R = status;
        } else {
            GPIO_PORTB_DATA_R = BLANK;
        }
    }
    GPIO_PORTB_DATA_R = status;


}
