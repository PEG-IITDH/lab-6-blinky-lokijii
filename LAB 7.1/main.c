#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

#define MASK_BITS 0x11                  // Bit mask for user switches SW1 and SW2

void GPIOF_config(void);
void GPIOF_Interrupt_config(void);

void main(void)
{
    SYSCTL_RCC_R &= ~(1 << 20);        // Disable the system clock as the source for PWM (16MHz)
    SYSCTL_RCGC0_R |= 0x00100000;     // Enable the clock for the PWM module
    SYSCTL_RCGCPWM_R = 0x3;           // Enable clock for PWM modules 1

    GPIOF_config();                   // Configure GPIO for use
    GPIOF_Interrupt_config();         // Configure GPIO interrupts
    GPIO_PORTF_AFSEL_R |= 0x02;       // Enable alternate function on PF1 (PWM signal)
    GPIO_PORTF_PCTL_R |= 0x50;        // Set M1PWM5 function on PF1

    PWM1_2_CTL_R = 0x00000000;        // Disable PWM module 1, generator 2
    PWM1_2_GENB_R |= 0x80E;           // Configure PWM module 1, generator 2, signal B: invert on compare B down count, high on load, low on zero
    PWM1_2_LOAD_R = 160;              // Set the load value for a 100kHz signal
    PWM1_2_CMPB_R = 80;              // Set the compare value for a 50% duty cycle
    PWM1_2_CTL_R |= 0x01;             // Enable the PWM block
    PWM1_ENABLE_R |= (1 << 5);       // Enable M1PWM5 signal on the pin

    /* Perform some operations */
    while (1);
}

void GPIOF_config()
{
    SYSCTL_RCGC2_R |= 0x20;           // Enable clock for port F
    GPIO_PORTF_LOCK_R = 0x4C4F434B;   // Unlock commit register
    GPIO_PORTF_CR_R = 0x1F;           // Make PORTF0 configurable
    GPIO_PORTF_DEN_R = 0x1F;          // Enable digital I/O on all PF pins
    GPIO_PORTF_DIR_R = 0x0E;          // Set PF0 and PF4 as input, PF1-PF3 as output
    GPIO_PORTF_PUR_R = 0x11;          // Enable pull-up resistor on PF0 and PF4
}

void GPIOF_Interrupt_config()
{
    GPIO_PORTF_IM_R &= ~MASK_BITS;    // Mask interrupts for SW1 and SW2
    GPIO_PORTF_IS_R &= ~MASK_BITS;    // Configure edge-sensitive interrupts
    GPIO_PORTF_IEV_R &= ~MASK_BITS;   // Configure falling edge to trigger the interrupt
    GPIO_PORTF_IBE_R &= ~MASK_BITS;   // Configure interrupt control using GPIOIEV
    NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF1FFFFF) | (2 << 21);  // Set priority of interrupt 30 (Port F) to 2
    NVIC_EN0_R |= (1 << 30);          // Enable interrupt on Port F
    GPIO_PORTF_ICR_R = MASK_BITS;     // Clear Raw Interrupt Status (RIS) and Masked Interrupt Status (MIS) for edge-sensitive interrupt
    GPIO_PORTF_IM_R |= MASK_BITS;     // Unmask interrupts for SW1 and SW2
}

void GPIOF_INT_Handler(void)
{
    GPIO_PORTF_IM_R &= ~MASK_BITS;    // Mask interrupts for SW1 and SW2
    int i;
    if (GPIO_PORTF_RIS_R & 0x01)     // Check if the interrupt is caused by SW2
    {
        for (i = 0; i < 160000; i++) {}  // Implement a delay for debouncing
        if (~(GPIO_PORTF_DATA_R) & 0x01)  // If the input on SW2 is 0 (pressed), decrease the duty cycle (implemented for debouncing)
        {
            if (PWM1_2_CMPB_R < 152)
            {
                PWM1_2_CMPB_R -= 8;  // Decrease the duty cycle by 5%
            }
        }
    }

    if (GPIO_PORTF_RIS_R & 0x10)      // Check if the interrupt is caused by SW1
    {
        for (i = 0; i < 160000; i++) {}  // Implement a delay for debouncing
        if (~(GPIO_PORTF_DATA_R) & 0x10)  // If the input on SW1 is 0 (pressed), increase the duty cycle (implemented for debouncing)
        {
            if (PWM1_2_CMPB_R > 8)
            {
                PWM1_2_CMPB_R += 8;  // Increase the duty cycle by 5%
            }
        }
    }
    GPIO_PORTF_ICR_R = MASK_BITS;     // Clear the interrupt for Port F
    GPIO_PORTF_IM_R |= MASK_BITS;     // Unmask interrupts for SW1 and SW2
}
