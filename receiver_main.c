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
   TA1CCTL1 = OUTMOD_3;             // set to OUTMOD_3 for pwm set/reset
   TA1CCTL2 = OUTMOD_3;             // set to OUTMOD_3 for pwm set/reset
   TA1CCR0 = 0x1F;                  // set to 0x1F
   TA1CCR1 = 0x00;                  // set to 0x00
   TA1CCR2 = 0x00;                  // set to 0x00
   TA1CTL = TASSEL_2 + MC_1 + ID_2 + TACLR; // set to use SMCLK (TASSEL_2) and count to CCR0 in up
                                    // mode (MC_1) divided by 4 (ID_2)

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
   // used in our communication and is checked to be 0 before accepting data
   if ((serialReg & 128) == 0)
   {
      // if the potentiometer is left of center
      if (((serialReg & 0x3F) >= 0) && ((serialReg & 0x3F) <= 0x1F))
      {
          // set PWM for servos
          TA1CCR1 = 0;
          TA1CCR2 = 0x1F - (serialReg & 0x1F);
      }
      // if the potentiometer is right of center
      else
      {
          // set PWM for servos
          TA1CCR1 = serialReg & 0x1F;
          TA1CCR2 = 0;
      }
   }
}
