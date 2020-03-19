/*
 * FlashStatus.h
 *
 *  Created on: Mar 17, 2020
 *      Author: Caleb
 */

#ifndef FLASHSTATUS_H_
#define FLASHSTATUS_H_

void FlashStatus(unsigned long status);

volatile extern unsigned int flash_status;
volatile extern unsigned int flash_count;
volatile extern unsigned long flashtickCount;

#define ONE 0x10
#define TWO 0x20
#define THREE 0x30
#define FOUR 0x40
#define FIVE 0x50
#define SIX 0x60
#define SEVEN 0x70
#define EIGHT 0x80
#define NINE 0x90
#define BLANK 0xF0

#define TRUE 1
#define FALSE 0

#define FLASH_TICK_MAX 50
#define FLASH_COUNT_MAX 4



#endif /* end of FLASHSTATUS_H_ */
