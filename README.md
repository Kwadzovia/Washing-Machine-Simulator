This project includes design and implementation of a real-time embedded control system for a washing machine simulator. The hardware used for this project includes the 
Bytronic Washing Machine Simulator (WMS) to represent a realistic washing machine which includes electronics such as a 7-segment display driver, motor drive and direction 
control, buzzer, switch input set/reset latch circuitry and an infra-red speed sensor. The WMS will be integrated with the TM4C123 Launch Pad as the microcontroller.  

The WMS is sectioned into two components: the circuit board which includes all the electronics and the display which represents what a typical user would be presented.
This project will include a user-friendly display which allows the user to cycle through different pre-set states which the user can select and start. After the cycle
begins, the current status will be displayed on the seven-segment display and the speed of the DC motor will be varied depending on the type of wash that was selected.
Upon completion, a buzzer will sound to indicate the cycle has been completed. 

In addition to the regular operation, a series of emergency interrupts will be implemented to handle special cases, such as a washing machine door opening in the middle
of a cycle or a user manually cancelling the run before completion. 

This project will provide an excellent introduction to the implementation of a real-time embedded control system and allow students to practice handling many different
states of a system depending on the inputs provided by a user. 