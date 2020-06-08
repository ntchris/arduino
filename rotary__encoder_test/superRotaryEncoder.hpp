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

class SuperRotaryEncoder {
    static const int IntUnknown = 0, IntA = 1, IntPush = 2, DirUnknown = 0, DirRight = 1, DirLeft = 2;

    static int encoderValue;
  public:
    static int _rotaryEncoderA,  _rotaryEncoderB;
    static bool debug;
    SuperRotaryEncoder(int pinA, int pinB);

    static void sleepMicroSec(unsigned long microsec);
    static bool checkIfDebouncePass(int a, int b);


    static unsigned long getTimeStamp();

    void displayValues();

    static int getEncoderValueSumAccel();

    static void processEncoderInterrupt();
    int getValue();
    void selftest ();
};

















#endif
