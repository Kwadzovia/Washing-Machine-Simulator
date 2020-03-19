#include "SysTickInts.h"
#include "PLL.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

#include "ProgramSelect.h"
//#include "WashTimer.h"
#include "FlashStatus.h"
#include "WashCycle.h"

void Full_Port_Init(void);
void PwmInit(void);
void disable_interrupts(void);
void enable_interrupts(void);
void wait_for_interrupts(void);
void ResetSwitches(void);

volatile unsigned int program = 0; //Used in Program Select
volatile unsigned long systickCount = 1000;
volatile unsigned long flashtickCount = 500;
volatile unsigned int washCount; // Used by wash timer
volatile unsigned int accept_flag = 0; // Not used?
volatile unsigned int flash_status = 1; // Used by FlashStatus
volatile unsigned int flash_count = 0; // Used by FlashStatus



void PortF_Interrupt_Handler(void);
void PortA_Interrupt_Handler(void);

volatile float duty_cycle = 0.1;


/* main */
int main(){
  PLL_Init(); // bus clock at 80 MHz
  Full_Port_Init(); // Full port initialization
  ResetSwitches(); // Reset switches before we begin

  SysTick_Init(80000);        // initialize SysTick timer

  //PwmInit();
  enable_interrupts();

  GPIO_PORTA_ICR_R = 0xFF;

  // Display nothing on the 7-segment
  GPIO_PORTB_DATA_R = 0xFF;

  while(TRUE) {
      //STEP 1: PROGRAM SELECT
      // Remains in an infinite loop until a program has been
      // confirmed as selected
      program = Program_Select();

      // STEP 2: WASH CYCLE
      // Runs through the wash cycle using the settings that
      // were previously selected.
      WashCycle(program);
  }
  while(1){                    // interrupts every 1ms
      wait_for_interrupts();
  }
}


/* Initialize PortF GPIOs */
void Full_Port_Init(void) {

    //Port A
    // Port A manages the Program selectors, Accept and Cancel buttons
    SYSCTL_RCGC2_R |= 0x00000001;           // activate clock for PortA
    while ((SYSCTL_PRGPIO_R & 0x00000001) == 0)
    {};                          // wait until PortA is ready
    GPIO_PORTA_LOCK_R = 0x4C4F434B;         // unlock GPIO PortA
    GPIO_PORTA_CR_R = 0xEC;                 // allow changes to PA7-5, PA2-3
    GPIO_PORTA_AMSEL_R = 0x00;              // disable analog on PortA
    GPIO_PORTA_PCTL_R = 0x00;               // use pins as GPIO
    GPIO_PORTA_DIR_R = 0x00;                // PA7,PA6,PA5, PA2, PA3 In | IN = 0, OUT = 1
    GPIO_PORTA_AFSEL_R = 0x00;              // disable alt function on PA
    GPIO_PORTA_PUR_R = 0xEC;                // Enable pull up resistors on PA
    GPIO_PORTA_DEN_R = 0xEC;                // enable digital I/O on PA7-5, 3, 2

    //Port B
    SYSCTL_RCGC2_R |= 0x00000002;           // activate clock for PortC
    while ((SYSCTL_PRGPIO_R & 0x00000002) == 0)
    {};                          // wait until PortB is ready
    GPIO_PORTB_LOCK_R = 0x4C4F434B;         // unlock GPIO PortB
    GPIO_PORTB_CR_R = 0xF0;                 // allow changes to PB7-4
    GPIO_PORTB_AMSEL_R = 0x00;              // disable analog on PortB
    GPIO_PORTB_PCTL_R = 0x00;               // use pins 7-4 as GPIO
    GPIO_PORTB_DIR_R = 0xF0;                // PB7-4 out | IN = 0, OUT = 1
    GPIO_PORTB_AFSEL_R = 0x00;              // disable alt function on PB
    GPIO_PORTB_PUR_R = 0xF0;                // Needs a pullup resistor
    GPIO_PORTB_DEN_R = 0xF0;                // enable digital I/O on PB7-4

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
    GPIO_PORTF_CR_R = 0x17;                 // allow changes to PF4-0
    GPIO_PORTF_AMSEL_R = 0x00;              // disable analog on PortF
    GPIO_PORTF_PCTL_R = 0x00;               // use pins as GPIO
    GPIO_PORTF_DIR_R = 0x05;                // PF4,PF1 in, PF0, PF2 Out | IN = 0, OUT = 1
    GPIO_PORTF_AFSEL_R = 0x00;              // disable alt function on PF
    GPIO_PORTF_PUR_R = 0x12;                // enable pull-up on PF1,PF4
    GPIO_PORTF_DEN_R = 0x1F;                // enable digital I/O on PF0,PF1,PF2,PF4

    // I don't think we need these interrupts set for PF or PA
    //Interrupt Setup (NVIC = Nested Vector Interrupt Controller)
//    GPIO_PORTF_IM_R = 0x00;                 // Mask All Interrupts to prevent Firing during setup
//    GPIO_PORTF_IS_R = 0x00;                 // Make PortF interrupts Edge Sensitive
//    GPIO_PORTF_IBE_R = 0x00;                // Make interrupts sensitive to one edge only
//    GPIO_PORTF_IEV_R = ~0x11;               // Make PF4 and PF0 sensitive to falling edge
//    GPIO_PORTF_ICR_R = 0x11;                // Clear PF4 and PF0 Interrupts Flag
//    GPIO_PORTF_IM_R = 0x11;                 // Unmask PF4 and PF0, done setup
//    NVIC_EN0_R |= 0x40000000;                // Enable NVIC Pin 31: PORT F

    //Program Select Interrupts
//    GPIO_PORTA_IM_R = 0x00;                 // Mask All Interrupts to prevent Firing during setup
//    GPIO_PORTA_IS_R = 0x00;                 // Make PortA interrupts Edge Sensitive
//    GPIO_PORTA_IBE_R = 0x00;                // Make interrupts sensitive to one edge only
//    GPIO_PORTA_IEV_R = 0x00;               // Make PA7-5 PA3-2,sensitive to falling edge
//    GPIO_PORTA_ICR_R = 0xFF;                // Clear PA7-5 PA3-2,Interrupts Flag
//    GPIO_PORTA_IM_R = 0xEC;                 // Unmask PA7-5 PA3-2, done setup
//    NVIC_EN0_R = 0x00000001;                       // Enable NVIC Pin 0: PORT A

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
    
    systickCount = systickCount - 1;
    if (systickCount <= 0){

        systickCount = 1000;

        // Used by wash timer
        if(washCount > 0)
        {
            washCount = washCount - 0x10;
        }
        else
        {
            washCount = 0;
        }
    }

    flashtickCount = flashtickCount - 1;
    if (flashtickCount <= 0) {

        flashtickCount = FLASH_TICK_MAX;

        if (flash_count > 0 && flash_status == FALSE) {
            flash_count -= 1;
            flash_status = TRUE;
        } else {
            flash_status = FALSE;
        }
    }
	
}

void PortF_Interrupt_Handler(void){

}

void PortA_Interrupt_Handler(void)
{

}
