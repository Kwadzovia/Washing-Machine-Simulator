/*
 * WashCycle.h
 *
 *  Created on: Mar 17, 2020
 *      Author: Caleb
 */

#ifndef WASHCYCLE_H_
#define WASHCYCLE_H_

void WashCycle(volatile unsigned long program);

#define WHITE 0x00
#define COLOUR 0x01
#define MIXED 0x02
#define ECONOMY 0x03
#define DELICATES 0x04
#define HEAVY 0x05

#define HIGH 0x90
#define MEDIUM 0x50
#define LOW 0x20

#endif /* end of WASHCYCLE_H_ */
