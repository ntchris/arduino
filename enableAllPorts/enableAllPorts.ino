/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified  8 May 2014
  by Scott Fitzgerald
 */


const int MinDigitPort = 2;
const int MaxDigitPort = 13;

// the setup function runs once when you press reset or power the board
void setup() {
    
    int i ;
   for(i= MinDigitPort; i <= MaxDigitPort; i++)
    { 
        pinMode(i, OUTPUT);
    }
    
}


void turnAllOn()
{
    int i;
 for(i = MinDigitPort; i <= MaxDigitPort; i++)
    {
        digitalWrite(i, HIGH);   // turn the LED on (HIGH is the voltage level)
    
    }
}

void turnAllOff()
{
    
    int i;
 for( i= MinDigitPort; i <= MaxDigitPort; i++)
    {
        digitalWrite(i, LOW);   // turn the LED on (HIGH is the voltage level)
    
    }
}

// the loop function runs over and over again forever
void loop() {
    int i;
   for( i = MinDigitPort; i <= MaxDigitPort; i++)
    {
       turnAllOn();
       delay(500);   
       turnAllOff();
       delay(500); 
    }
}