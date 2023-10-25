#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

#define MASK_BITS   0x11  // Bit mask for user switch 1 and 2

int main(void)
{
    // Configure GPIO PORT F
    SYSCTL_RCGC2_R |= 0x00000020;     // Enable clock for GPIOF
    GPIO_PORTF_LOCK_R = 0x4C4F434B;   // Unlock the commit register
    GPIO_PORTF_CR_R = 0x01;           // Make PORTF0 configurable
    GPIO_PORTF_DEN_R = 0x1F;          // Enable digital I/O on all PF pins
    GPIO_PORTF_DIR_R = 0x0E;          // Set PF4 pin as input (user switch pin)
    GPIO_PORTF_PUR_R = 0x11;          // Enable pull-up resistor on PF0 and PF4

    // Configure GPIO PORT F interrupts
    GPIO_PORTF_IM_R &= ~MASK_BITS;    // Mask interrupts for SW1 and SW2
    GPIO_PORTF_IS_R &= ~MASK_BITS;    // Configure edge-sensitive interrupts
    GPIO_PORTF_IEV_R &= ~MASK_BITS;   // Falling edge triggers the interrupt
    GPIO_PORTF_IBE_R &= ~MASK_BITS;   // Interrupt is controlled by GPIOIEV
    NVIC_EN0_R |= (1 << 30);          // Enable interrupt on port F
    GPIO_PORTF_ICR_R = MASK_BITS;     // Clear Raw Interrupt Status (RIS) and Masked Interrupt Status (MIS) for edge-sensitive interrupt
    GPIO_PORTF_IM_R |= MASK_BITS;     // Unmask interrupts for SW1 and SW2

    // Configure UART7
    SYSCTL_RCGCUART_R |= 0x80;        // Enable UART module 7
    SYSCTL_RCGC2_R |= 0x10;           // Port E receives clock
    GPIO_PORTE_DEN_R |= 0x03;
    GPIO_PORTE_DIR_R |= 0x02;
    GPIO_PORTE_AFSEL_R |= 0x03;       // Port E0 and E1 alternate function m
    GPIO_PORTE_PCTL_R |= 0x000011;    // UARTRx (Tx) selected on PE0 (1)

    UART7_CTL_R &= 0xFFFFFFFE;         // Disable UART
    UART7_IBRD_R = 104;
    UART7_FBRD_R = 11;
    UART7_LCRH_R |= 0x62;             // 8-bit data, odd parity enabled
    UART7_CC_R &= 0xFFFFFFF0;          // System clock is the source for baud
    UART7_CTL_R |= (1 << 0) | (1 << 8) | (1 << 9); // Enable UART, transmit, and receive

    // Enable UART7 receive interrupt
    UART7_IM_R &= 0x00;               // Mask UART interrupts
    UART7_ICR_R &= 0x00;              // Clear all UART interrupts
    UART7_IM_R |= (1 << 4);           // Unmask UART receive interrupt
    NVIC_EN1_R |= (1 << 31);           // Enable UART7 interrupt in NVIC

    while (1) {}

    return 0;
}

void GPIOF_INT_Handler(void)
{
    GPIO_PORTF_IM_R &= ~MASK_BITS;   // Mask interrupts for SW2
    int i;

    if (GPIO_PORTF_RIS_R & 0x01)    // Interrupt on user switch 2
    {
        for (i = 0; i < 16000; i++) {} // Debounce delay
        if (~(GPIO_PORTF_DATA_R) & 0x01) // If input on switch 2 is 0
        {
            UART7_DR_R = 0xAA;           // Transmit hexadecimal AA on UART
        }
    }

    if (GPIO_PORTF_RIS_R & 0x10)    // Interrupt on user switch 1
    {
        for (i = 0; i < 16000; i++) {} // Delay for debounce
        if (~(GPIO_PORTF_DATA_R) & 0x10) // If input on switch 1 is 0
        {
            UART7_DR_R = 0xF0;           // Transmit hexadecimal F0 on UART
        }
    }

    GPIO_PORTF_ICR_R = MASK_BITS;   // Clear the interrupts
    GPIO_PORTF_IM_R |= MASK_BITS;   // Unmask interrupts
}

void UART7_Handler(void)
{
    UART7_IM_R &= 0x00;  // Mask receive interrupt

    if (UART7_FR_R & (1 << 6)) // if all data is recieved
    {
        if (UART7_DR_R == 0xF0)  // Check if received data is F0
        {
            GPIO_PORTF_DATA_R |= 0x04; // Turn on the blue LED
        }
        else if (UART7_DR_R == 0xAA) // Check if received data is AA
        {
            GPIO_PORTF_DATA_R |= 0x08; // Turn on the green LED
        }
        else if (UART7_RSR_R & 0xF) // Check for errors
        {
            GPIO_PORTF_DATA_R |= 0x02; // Turn on the red LED
        }
    }
    int i;
    for (i = 0; i < 800000; i++) {} // Delay
    GPIO_PORTF_DATA_R &= 0x00;      // Turn off the LEDs
    UART7_ECR_R &= ~(0xF);          // Clear the error bits
    UART7_ICR_R &= 0x00;            // Clear UART interrupt flags
    UART7_IM_R |= (1 << 4);         // Unmask UART receive interrupt
}
