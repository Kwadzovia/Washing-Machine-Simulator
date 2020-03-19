/*
 * WashCycle.c
 *
 *  Created on: Mar 17, 2020
 *      Author: Caleb
 */
// Runs through an entire wash cycle. Takes the program selected as the input
// which determines the run time for things like heat and wash stages.

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FlashStatus.h"
#include "WashTimer.h"
#include "WashCycle.h"
#include "MotorFunctions.h"


unsigned long GetWashTime(unsigned long program);
unsigned long GetHeatTime(unsigned long program);
unsigned long GetWashSpeed(unsigned long program);

void WashCycle(unsigned long program) {
    volatile unsigned long washTime = GetWashTime(program);
    volatile unsigned long heatTime = GetHeatTime(program);
    volatile float washSpeed = GetWashSpeed(program);

    // Stage 1: Empty
    FlashStatus(ONE);
    WashTimer(MAX_TIME);

    // Stage 2: Fill
    FlashStatus(TWO);
    WashTimer(MAX_TIME);

    // Stage 3: Heat
    FlashStatus(THREE);
    WashTimer(heatTime);

    // Stage 4: Wash
    FlashStatus(FOUR);
    StartMotor(washSpeed);
    WashTimer(washTime);
    StopMotor();

    // Stage 5: Rinse
    FlashStatus(FIVE);
    StartMotor(washSpeed);
    WashTimer(MAX_TIME);

    // Stage 6: Spin
    FlashStatus(SIX);
    StartMotor(MOTOR_MAX);
    WashTimer(MAX_TIME);
    StopMotor();

    // Stage 7: Dry
    FlashStatus(SEVEN);
    WashTimer(MAX_TIME);

    // Stage 8: Complete
    FlashStatus(EIGHT);
}

unsigned long GetHeatTime(unsigned long program) {

    // Remove any unnecessary bits
    program &= 0x0F;

    if (program == WHITE) {
        return HIGH;
    } else if (program == COLOUR) {
        return MEDIUM;
    } else if (program == MIXED) {
        return MEDIUM;
    } else if (program == ECONOMY) {
        return LOW;
    } else if (program == DELICATES) {
        return LOW;
    } else {
        return HIGH;
    }
}

unsigned long GetWashTime(unsigned long program) {

    // Remove any unnecessary bits
    program &= 0x0F;

    if (program == WHITE) {
        return MEDIUM;
    } else if (program == COLOUR) {
        return MEDIUM;
    } else if (program == MIXED) {
        return HIGH;
    } else if (program == ECONOMY) {
        return LOW;
    } else if (program == DELICATES) {
        return LOW;
    } else {
        // Heavy load
        return HIGH;
    }
}

unsigned long GetWashSpeed(unsigned long program) {

    // Remove any unnecessary bits
    program &= 0x0F;

    if (program == WHITE) {
        return MOTOR_MID;
    } else if (program == COLOUR) {
        return MOTOR_MID;
    } else if (program == MIXED) {
        return MOTOR_MAX;
    } else if (program == ECONOMY) {
        return MOTOR_MIN;
    } else if (program == DELICATES) {
        return MOTOR_MIN;
    } else {
        // Heavy load
        return MOTOR_MAX;
    }
}


