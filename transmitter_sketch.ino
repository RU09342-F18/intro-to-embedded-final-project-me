/**
 * sketch.ino
 * Author: Ryan Baker
 * Date: 11/30/18
 */

int analogVal;                      // initialize variable to store the value from the ADC
byte serialReg;                     // initialize variable to store the byte to be transmitted
byte lastReg;                       // initialize variable to store last value of serialReg

void setup()
{
   Serial1.begin(9600);             // begin UART at 9600 baud
   analogReadResolution(12);        // set ADC to 12-bit resolution
   lastReg = 64;                    // 01000000 in binary
}

void loop()
{
   serialReg = 64;                  // 01000000 in binary
   analogVal = analogRead(P6_0);    // read analog value from P6.0
   serialReg = (serialReg | (analogVal / 64)) & 254; // 01XXXXX0 in binary
   // if the value has changed
   if (serialReg != lastReg)
   {
      Serial1.write(serialReg);     // send the new value
      lastReg = serialReg;          // store current serialReg for next comparison
   }
   delay(10);
}
