#include "SysTickInts.h"
#include "PLL.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

#include "ProgramSelect.h"

void Full_Port_Init(void);
void PwmInit(void);
void disable_interrupts(void);
void enable_interrupts(void);
void wait_for_interrupts(void);

volatile unsigned long count = 0;
volatile unsigned long In, Out;



void SW1_Handler(void);
volatile float duty_cycle = 0.1;


/* main */
int main(void){
  PLL_Init();                 // bus clock at 80 MHz
  Full_Port_Init();

  SysTick_Init(80000);        // initialize SysTick timer

  PwmInit();
  enable_interrupts();

  while(1){                   // interrupts every 1ms
      wait_for_interrupts();
  }
}


/* Initialize PortF GPIOs */
void Full_Port_Init(void) {

    //Port A
    SYSCTL_RCGC2_R |= 0x00000001;           // activate clock for PortA
    while ((SYSCTL_PRGPIO_R & 0x00000001) == 0)
    {};                          // wait until PortA is ready
    GPIO_PORTA_LOCK_R = 0x4C4F434B;         // unlock GPIO PortA
    GPIO_PORTA_CR_R = 0xE0;                 // allow changes to PA7-5
    GPIO_PORTA_AMSEL_R = 0x00;              // disable analog on PortF
    GPIO_PORTA_PCTL_R = 0x00000000;         // use pins as GPIO
    GPIO_PORTA_DIR_R = 0xE0;                // PA7,PA6,PA5 Out
    GPIO_PORTA_AFSEL_R = 0x00;              // disable alt function on PA
    GPIO_PORTA_PUR_R = 0x00;                // no inputs, no pullups
    GPIO_PORTA_DEN_R = 0xE0;                // enable digital I/O on PA7-5

    //Port B
    SYSCTL_RCGC2_R |= 0x00000002;           // activate clock for PortB
    while ((SYSCTL_PRGPIO_R & 0x00000002) == 0)
    {};                          // wait until PortB is ready
    GPIO_PORTB_LOCK_R = 0x4C4F434B;         // unlock GPIO PortB
    GPIO_PORTB_CR_R = 0x0F;                 // allow changes to PB3-PB0
    GPIO_PORTB_AMSEL_R = 0x00;              // disable analog on PortB
    GPIO_PORTB_PCTL_R = 0x00000000;         // use pins as GPIO
    GPIO_PORTB_DIR_R = 0x0F;                // PB3,PB2,PB1,PB0 out
    GPIO_PORTB_AFSEL_R = 0x00;              // disable alt function on PB
    GPIO_PORTB_PUR_R = 0x00;                // no inputs, no pullups
    GPIO_PORTB_DEN_R = 0x0F;                // enable digital I/O on PB3-PB0

    //Port E
    SYSCTL_RCGC2_R |= 0x00000010;           // activate clock for PortE
    while ((SYSCTL_PRGPIO_R & 0x00000010) == 0)
    {};                          // wait until PortE is ready
    GPIO_PORTE_LOCK_R = 0x4C4F434B;         // unlock GPIO PortE
    GPIO_PORTE_CR_R = 0x30;                 // allow changes to PE5, PE4
    GPIO_PORTE_AMSEL_R = 0x00;              // disable analog on PortE
    GPIO_PORTE_PCTL_R = 0x00000000;         // use pins as GPIO
    GPIO_PORTE_DIR_R = 0x30;                // PE5,PE4 Out
    GPIO_PORTE_AFSEL_R = 0x00;              // disable alt function on PE
    GPIO_PORTE_PUR_R = 0x00;                // no inputs, no pullups
    GPIO_PORTE_DEN_R = 0x30;                // enable digital I/O on PE5, PE4

    //Port F
    SYSCTL_RCGC2_R |= 0x00000020;           // activate clock for PortF
    while ((SYSCTL_PRGPIO_R & 0x00000020) == 0)
    {};                          // wait until PortF is ready
    GPIO_PORTF_LOCK_R = 0x4C4F434B;         // unlock GPIO PortF
    GPIO_PORTF_CR_R = 0x13;                 // allow changes to PF4-0
    GPIO_PORTF_AMSEL_R = 0x00;              // disable analog on PortF
    GPIO_PORTF_PCTL_R = 0x00000000;         // use pins as GPIO
    GPIO_PORTF_DIR_R = 0x0E;                // PF4,PF1,PF0 in
    GPIO_PORTF_AFSEL_R = 0x00;              // disable alt function on PF
    GPIO_PORTF_PUR_R = 0x13;                // enable pull-up on PF0,PF1,PF4
    GPIO_PORTF_DEN_R = 0x00;                // enable digital I/O on PF0,PF1,PF4

    GPIO_PORTF_DATA_R ^= 0x02;              // Start LED as red

    //Interrupt Setup (NVIC = Nested Vector Interrupt Controller)
    GPIO_PORTF_IM_R = 0x00;                 // Mask All Interrupts to prevent Firing during setup
    GPIO_PORTF_IS_R = 0x00;                 // Make PortF interrupts Edge Sensitive
    GPIO_PORTF_IBE_R = 0x00;                // Make interrupts sensitive to one edge only
    GPIO_PORTF_IEV_R = ~0x10;               // Make PF4 sensitive to falling edge
    GPIO_PORTF_ICR_R = 0x10;                // Clear PF4 Interrupts Flag
    GPIO_PORTF_IM_R = 0x10;                 // Unmask PF4, done setup
    NVIC_EN0_R |= 0x40000000;                // Enable NVIC Pin 31: PORT F

}

void PwmInit(void){
//    //Following Shukra Info with changes to work with 80 MHz on Port E
//
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
    PWM0_2_LOAD_R = 50000;                      // Formula from Shukra
    PWM0_2_CMPA_R = (1-duty_cycle) * PWM0_2_LOAD_R;     // formula for increment by 10%
    PWM0_2_CTL_R = 1;                           // Start counter
    PWM0_ENABLE_R = 0x10;                       // Enable PWM0 Channel 4



}



/* Disable interrupts by setting the I bit in the PRIMASK system register */
void disable_interrupts(void) {
    __asm("    CPSID  I\n"
          "    BX     LR");
}


/* Enable interrupts by clearing the I bit in the PRIMASK system register */
void enable_interrupts(void) {
    __asm("    CPSIE  I\n"
          "    BX     LR");
}


/* Enter low-power mode while waiting for interrupts */
void wait_for_interrupts(void) {
    __asm("    WFI\n"
          "    BX     LR");
}


/* Interrupt service routine for SysTick Interrupt */
// Executed every 12.5ns*(period)
void SysTick_Handler(void){
    
	
}

void SW1_Handler(void){
    //Clear Interrupts to prevent triggering again
    GPIO_PORTF_ICR_R = 0x10;                // Clear PF4 Interrupts Flag

    // Toggle LED Between red and blue just to its working
    GPIO_PORTF_DATA_R ^= 0x06;

    // Increase Duty Cycle
    duty_cycle += 0.1;
    // Reset at 100%
    if(duty_cycle >= 1.0){
        duty_cycle = 0;
    }
    PWM0_2_CMPA_R = (1-duty_cycle) * PWM0_2_LOAD_R; //Change PWM ratio



}

