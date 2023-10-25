# Embedded Systems Lab Report

<!-- Insert your details here -->
* Lokesh Kumar [ee23mt012@iitdh.ac.in] 
* Devendra Singh [ee23mt004@iitdh.ac.in]
* Group: 09

### Problem Statement:

Generate a 50% duty cycle PWM waveform on TM4C123GH6PM, change the duty cycle of the signal on the press of on-board user switches

### Procedure:

#### Task 1: 
##### Create a PWM waveform with frequency = 100KHz and variable duty cycle.

The program should begin with d = 50%.

On pressing one switch the duty should be increased by 5% and on pressing other switch it should be decreased by 5%.

Enable System Clock as Source for PWM Modules:
Use the RCC (Reset and Clock Control) registers to configure the system clock source for the PWM modules. This typically involves setting the appropriate bits in these registers to select the system clock as the source for the PWM.

Enable Clock and Initialize GPIO Port F:
Enable the clock for the specific GPIO port you'll be using (e.g., Port F) by setting the appropriate bits in the RCGC (Run Mode Clock Gating Control) registers. Initialize GPIO Port F for the desired functionality and direction, ensuring it's set up for PWM output.

Enable Interrupts on On-Board Switches:
Configure and enable interrupts for the on-board switches, like user switch 1 and 2. This involves modifying registers to set the interrupt conditions and enable the interrupt lines.

Enable Alternate Function for PWM Output Pin:
Set the Alternate Function Select (AFSEL) bits for the GPIO pin where you want to generate the PWM signal. Additionally, configure the Port Control (PCTL) registers to select the PWM function for that pin.

Configure and Enable PWM Generator:
Disable the PWM generator first, then load the initial 'load' and 'compare' values into their respective registers to determine the PWM period and initial duty cycle. Configure the PWM generator settings, including the PWM_GENA or PWM_GENB registers, to specify the PWM mode and behavior. Finally, enable the PWM generator and assign the PWM signal to the desired GPIO pin.

Dynamically Adjust Duty Cycle on User Switch Press:
When the user presses switch 1 or 2, modify the 'compare' value associated with the PWM generator. Increasing or decreasing this value will effectively change the duty cycle of the PWM signal, adjusting the signal's on-time and brightness or other controlled parameters.

#### Task 2:
##### Implement the same but using only 1 switch (SW1 OR SW2) – short press for d increase and long press for decrease.

Enable System Clock as Source for PWM Modules:
Utilize the Reset and Clock Control (RCC) registers to designate the system clock as the source for PWM modules. Adjust the appropriate bits within these registers to configure the system clock as the source for PWM operations.

Enable Clock and Initialize GPIO Port F:
Activate the clock for the specific GPIO port you intend to use, such as Port F, by manipulating the Run Mode Clock Gating Control (RCGC) registers. Initialize GPIO Port F to match the desired functionality and direction, ensuring it's properly configured for PWM output.

Enable Interrupts on On-Board Switches:
Configure and enable interrupt capabilities for the on-board switches, like user switch 1 and 2. This entails configuring registers to establish interrupt conditions and enabling the respective interrupt lines.

Enable Alternate Function for PWM Output Pin:
Set the Alternate Function Select (AFSEL) bits for the GPIO pin where you want to generate the PWM signal. Additionally, configure the Port Control (PCTL) registers to select the PWM function for that pin.

Configure and Enable PWM Generator:
Initially, disable the PWM generator, then populate the 'load' and 'compare' values in their corresponding registers to define the PWM period and initial duty cycle. Customize the PWM generator settings, including the PWM_GENA or PWM_GENB registers, to specify the PWM mode and behavior. Finally, enable the PWM generator and route the PWM signal to the desired GPIO pin.

Manage Duty Cycle with SysTick Timer on User Switch Press (Falling Edge):
Upon detecting a falling edge, which occurs when the user switch is pressed, activate the SysTick timer and set a reload value in the 'STRELOAD' register that corresponds to a 0.5-second delay.

Adjust PWM Duty Cycle on User Switch Release (Rising Edge):
When the button is released and a rising edge is detected, examine the 'COUNT_FLAG' of the SysTick timer. If it's set, decrease the duty cycle of the PWM signal. If it's not set, increase the duty cycle of the PWM signal. Subsequently, turn off the SysTick timer.

### Measurements:

With 16MHz clock given to PWM module, to generate a 100kHz signal, we need to wait for 16MHz/100kHz clock cycles. Therefore, the load value for the PWM module is set to be 160.
To generate a PWM signal of 50 percent duty cycle, the compare value must be 160/2 = 80.
When increasing(decreasing) the duty cycle by 5% we must decrement(increment) the compare value by 8 units.

