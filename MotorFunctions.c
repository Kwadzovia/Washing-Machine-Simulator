/*
 * MotorFunctions.c
 *
 *  Created on: Mar 18, 2020
 *      Author: Caleb
 */

// This function is to run the motor

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "MotorFunctions.h"

void StartMotor(float duty_cycle) {
    PWM0_2_CMPA_R = (1.0-duty_cycle)*PWM0_2_LOAD_R;
    PWM0_ENABLE_R = START;
}

void StopMotor() {
    PWM0_ENABLE_R = STOP;

}

void ChangeMotorDirection() {

}

void PwmInit(){
//    //Following Shukra Info with changes to work with 80 MHz on Port E
    SYSCTL_RCGCPWM_R |= 0x01;                // Turn on clock for PWM Module 0
    SYSCTL_RCGCGPIO_R |= 0x10;               // Turn on clock for PortE
    SYSCTL_RCC_R &= 0x00160000;             // Turn on PWM Clock divider @16 divisions -> 5 MHz

    GPIO_PORTE_AFSEL_R = 0x10;              // enable alt function on PE4

    GPIO_PORTE_PCTL_R &= ~0x000F0000;       // Clearing Any Set Function at PE4
    GPIO_PORTE_PCTL_R |= 0x00040000;        // Alternate Function: PWM PE4
    GPIO_PORTE_DEN_R = 0x10;                // enable digital I/O on PE4

    //Setup PWM
    PWM0_2_CTL_R = 0;                       // PWM Generator 2 (PWM4 Pin)
    PWM0_2_GENA_R = 0x0000008C;             // Raise to high when reloading, clear when counter = CMPA

    //PWM VALUES
    PWM0_2_LOAD_R = 50000 - 1;                      // Formula from Shukra
    PWM0_2_CMPA_R = (1-MOTOR_MAX) * PWM0_2_LOAD_R;     // Default is max
    PWM0_2_CTL_R = 1;                           // Start the counter
    PWM0_ENABLE_R = STOP;                       // Disable for now PWM0 Channel 4
}
