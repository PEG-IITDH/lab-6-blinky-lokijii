# Embedded Systems Lab Report

<!-- Insert your details here -->
* Lokesh Kumar [ee23mt012@iitdh.ac.in] 
* Devendra Singh [ee23mt004@iitdh.ac.in]
* Group: 09
* [11-10-2023]

### Problem Statement:

Program your microontroller to transmit:

"F0" if SW1 is pressed

"AA" if SW2 is pressed 

over UART with baud rate 9600 and odd parity. 

Your program should also listen for incoming data on the UART with the same baud and parity config; if "AA" is received LED should be GREEN; if "F0" is recieved, the LED should be BLUE and if any error is detected LED should be RED. Test this by communicating with your neighboring group.

### Solution Summary:

This code configures a UART communication system and GPIO for a microcontroller. It sets up UART Module 5 to communicate at a baud rate of 9600 with odd parity. The system responds to two user switches (SW1 and SW2) by transmitting "F0" and "AA" when pressed, respectively.

The program also handles received data: if "AA" is received, it lights an LED in green, and if "F0" is received, it lights the LED in blue. In case of any UART errors, it lights the LED in red. A SysTick timer is used to reset the LED status after a brief period.

The code includes interrupt handling for both user switches and UART data, ensuring responsive and efficient operation. Overall, it establishes a communication system with user interaction and status indication through LEDs.

### Assumptions, Constraints and Requirements:

Assumptions:

Hardware Configuration: The code assumes that the microcontroller hardware is set up with UART Module 5 and GPIO Port F, and the specific pins (PE4 and PE5) are used for UART communication.
User Input: It assumes that user switches (SW1 and SW2) are connected to GPIO Port F for user interaction.
Constraints:

Hardware Limitations: The code is constrained by the capabilities and limitations of the chosen microcontroller, such as clock speed and available GPIO pins.
Baud Rate and Parity: The code is designed for a specific baud rate (9600) and odd parity, limiting compatibility with systems using different settings.
Requirements:

UART Configuration: The code requires proper configuration of UART Module 5 with specific baud rate and word length settings.
GPIO Setup: Proper configuration of GPIO Port F is necessary for user switch input and LED output.
Interrupt Handling: The code relies on interrupt handling for user switch presses, UART data reception, and SysTick timer to meet its functional requirements.
LED Indicators: LEDs are required for status indication (Red, Blue, Green) to provide feedback to the user.
Timing: The SysTick timer is essential for resetting the LED status, and it requires accurate timing for this function to work as intended.

### Block diagram / Flowchart:

![Screenshot (153)](https://github.com/PEG-IITDH/lab-6-blinky-lokijii/assets/144203218/d1ff0a3d-f40f-4da2-a307-1f0c1625be46)

### Discussion and Conclusions:

The provided code successfully configures a UART communication system and GPIO for user interaction with LEDs as status indicators. It handles user switch presses and transmits specific data ("F0" and "AA") as required. Additionally, it responds to received data by illuminating LEDs in the corresponding colors (Red, Blue, Green). The code uses SysTick timer for resetting LED status, ensuring a user-friendly experience.

However, there are some aspects to consider:

Specific Hardware: The code is highly dependent on the specific microcontroller hardware, GPIO pins, and UART module chosen. Any changes in hardware may require adjustments to the code.

Fixed Baud Rate: The code is designed for a fixed baud rate (9600) and odd parity. It may not be easily adaptable to systems with different baud rates or communication requirements.

Limited Error Handling: The code primarily handles UART data reception errors by setting the LED to Red. More advanced error handling and recovery mechanisms could be implemented for greater robustness.

Conclusion:

In conclusion, the code serves its purpose of creating a simple UART communication system with user interaction and LED status feedback. It meets the defined requirements and constraints. To enhance its versatility, developers should consider making it more adaptable to different hardware configurations and communication settings. Additionally, improving error handling and recovery would make it more robust in real-world applications. Overall, this code provides a solid foundation for UART communication in embedded systems but can be extended for more advanced use cases.

Unexpected Behavior:

During testing, unexpected behavior could arise from incorrect hardware connections, such as mismatched pins or malfunctioning components. Additionally, if the clock settings or UART configuration are not precise, it might result in unreliable communication or incorrect LED indications.
