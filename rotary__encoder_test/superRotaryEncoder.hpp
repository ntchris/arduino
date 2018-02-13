// arduino class /library for rotary encoder.
// auther   ntchris@hotmail.com   Feb 2018

// pin definition: pinA: pinB:  rotary encoder pin A or pin B. 
// if swap A B , turn CW or CCW trigger ++ or --.
// pinD , pin E  push button....so far this class doesn't handle the push button.
// just a push button.
// pin C and pin E connect GND


#ifndef SuperRotaryEncoder_h
#define SuperRotaryEncoder_h

#include "Arduino.h"

class SuperRotaryEncoder
{
  int RotaryEncoderA,  RotaryEncoderB;
   public:
   bool debug=false;
   SuperRotaryEncoder(int pinA, int pinB); 
    
   unsigned long getTimeStamp();
 
   void displayValues(); 
   
   int getEncoderValueSumAccel();
   
   int getEncoderValueSum();
   void selftest ();
};

















#endif


