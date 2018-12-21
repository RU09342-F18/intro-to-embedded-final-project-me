# Final Project: RC Car
A compact car and a wireless controller are made using two MSP430 Launchpads and Bluetooth communication. This was done with an MSP430G2553, an MSP430F5529, a potentiometer, two HC-05 Bluetooth modules, and four FS90R servos.

## Motivation
I was primarily motivated to build an RC car that had finer control over a conventional low-cost RC car. This included factors such as responsiveness and the number of unique turning radiuses achievable.

## Design Criteria
* Create a "steering wheel" using a potentiometer on the controller
* Communicate between the controller and the car over Bluetooth
* Control four servos on the car based on the potentiometer position

## Software Implementation
### MSP430G2553
The MSP430G2553 is the board used for the car. All the code for this board was written in Code Composer Studio and can be found in the file: receiver_main.c

This software implementation involved using UART to receive data from the Bluetooth module and hardware PWM to write various speeds to the left and right motors.

### MSP430F5529
The MSP430F5529 is the board used for the controller. All the code for this board was written in Energia and can be found in the file: transmitter_sketch.ino

This software implementation involved using UART to transmit data to the Bluetooth module and the ADC to read the voltage from the potentiometer.

## Hardware Implementation
### MSP430G2553
The car was made up of the MSP430G2553, an HC-05 Bluetooth module, four FS90R servos, and two batteries, one being a 4.8V off-the-shelf RC car battery powering the servos and the other being a portable phone charger to power the Launchpad via its USB port.

### MSP430F5529
The controller was made up of the MSP430F5529, an HC-05 Bluetooth module, and a potentiometer. This board was powered by my laptop, for a truly portable option, another portable phone charger could be used.

## How-To Software
### MSP430G2553
Create a new project in Code Composer that uses the MSP430G2553. I used CCS 8.2.0 for this. Add the code to your project by adding the file directly or copying and pasting the contents of the file into a new file. Debug and let CCS flash the code to your processor. If there are any updates, update your board. Once the code is running on your board, you can unplug it and move on to the hardware portion of the setup.

### MSP430F5529
Using Energia, open the sketch from the repo, or copy and paste the contents of the sketch into a new one. I used Energia 1.8.7E21 for this. Go to 'Tools' > 'Board:' and select the MSP430F5529. Then, go to 'Tools' > 'Port' and ensure that the COM port of your MSP430F5529 is selected. Next, click the upload button near the top left of the window. After the code has been flashed to the board, it can be unplugged, and you can move on to the hardware portion of the setup.

## How-to Hardware
### MSP430G2553
Connect P1.1 to the TXD pin on the HC-05, P1.2 to the RXD pin on the HC-05, as well as 5V and GND to the HC-05. Connect P2.1 and P2.4 to the data pins on the left and right FS90R servos (two servos per port). Connect the ground of the 4.8V RC car battery to the ground of the Launchpad, then connect the ground and 4.8V lines of the battery to the ground and power pins on all four FS90R servos. Power on the board via the USB port and everything should be working.

### MSP430F5529
Connect P3.4 to the TXD pin on the HC-05, P3.3 to the RXD pin on the HC-05, as well as 5V and GND to the HC-05. Connect the ends of the potentiometer to 3.3V and GND and the connect P6.0 to the center of the potentiometer. Power on the board via the USB port and everything should be working.
