#include <msp430.h>

/**
 * main.c
 * Author: Ryan Baker
 * Date: 11/29/18
 */

char serialReg;                     // initialize variable to store the byte received

void main(void)
{
   // disable watchdog timer
   WDTCTL = WDTPW + WDTHOLD;        // watchdog timer stopped by enabling bit 7 (WDTHOLD) and using
                                    // the password (WDTPW)

   // enable output to LEDs and turn them off
   P2SEL |= 0x12;                   // enable peripheral function on P2.1 and P2.4...
   P2SEL2 &= ~0x12;                 // ...by settings both of these registers
   P2DIR |= 0x12;                   // set P2.1 and P2.4 as outputs
   P2OUT &= ~0x12;                  // set P2.1 and P2.4 low

   // enable RXD/TXD
   P1SEL |= 0x06;                   // enable RXD/TXD function on P1.1 and P1.2...
   P1SEL2 |= 0x06;                  // ...by settings both of these registers

   // configure timer A1
   TA1CCTL1 = OUTMOD_7;             // set to OUTMOD_7 for pwm reset/set
   TA1CCTL2 = OUTMOD_7;             // set to OUTMOD_7 for pwm reset/set
   TA1CCR0 = 0x4E20;                // set to 20000 (20ms period)
   TA1CCR1 = 0x03E8;                // set to 1000 (1ms pulse)
   TA1CCR2 = 0x07D0;                // set to 2000 (2ms pulse)
   TA1CTL = TASSEL_2 + MC_1 + TACLR; // set to use SMCLK (TASSEL_2) and count to CCR0 in up
                                    // mode (MC_1)

   // configure UART... (UCA0CTL1) / ((UCA0BR1 x 256) + UCA0BR0 + (UCA0MCTL / 8)) = baud rate
   UCA0CTL1 |= UCSSEL_2;            // set to use SMCLK (UCSSEL_2)
   UCA0BR0 = 104;                   // set to 104
   UCA0BR1 = 0;                     // set to 0
   UCA0MCTL = UCBRS_1;              // set modulation pattern to 1/8
   UCA0CTL1 &= ~UCSWRST;            // initialize USCI
   UC0IE |= UCA0RXIE;               // enable USCI_A0 RX interrupt

   // put into low power mode and enable global interrupts
   _BIS_SR(LPM0_bits + GIE);
}

#pragma vector=USCIAB0RX_VECTOR
// interrupt when receiving data
__interrupt void USCI0RX_ISR(void)
{
   serialReg = UCA0RXBUF;           // store the value

   // BT module outputs 0xFF before connection is established so the most significant bit is not
   // used in the communication and is checked to be 0 before accepting data
   if ((serialReg & 128) == 0)
   {
      // if the potentiometer is left of center
      if (((serialReg & 0x3F) >= 0) && ((serialReg & 0x3F) <= 0x1F))
      {
          // set PWM for servos
          TA1CCR1 = 0x07D0 - (serialReg & 0x1F)*32; // value between 0x03E8 and 0x07D0;
          TA1CCR2 = 0x07D0;
      }
      // if the potentiometer is right of center
      else
      {
          // set PWM for servos
          TA1CCR1 = 0x03E8;
          TA1CCR2 = 0x07D0 - (serialReg & 0x1F)*32; // value between 0x03E8 and 0x07D0;
      }
   }
}
