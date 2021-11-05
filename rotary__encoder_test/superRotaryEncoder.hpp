// arduino class /library for rotary encoder.
// auther   ntchris@hotmail.com   Feb 2018

// pin definition: pinA: pinB:  rotary encoder pin A or pin B.
// if swap A B , turn CW or CCW trigger ++ or --.

// to easily handle debounce, add capacitor from pin A to Gnd,
// and from pin B to Gnd, pinButton to Gnd.
//  value from 20nf to 100nf should work well.

#ifndef SuperRotaryEncoder_h
#define SuperRotaryEncoder_h

#include "Arduino.h"


class SuperRotaryEncoder {
    static const int FastStepFactor = 2;
    //static const ButtonHoldTimeSec
    static SuperRotaryEncoder *encoderInstance;
    static  int _fastRotateMs ;

    volatile int encoderValue;
    int encStep;
    int valueA, valueB;
    int minValue, maxValue;
    volatile int buttonStatus; // LOW: push down, HIGH: released
    volatile int isButtonClicked;
  public:

    int _rotaryEncoderPinA,  _rotaryEncoderPinB, _rotaryEncoderPinButton;
    static bool debug;

    // for atmega328 3 PORTs, different pins use different port to trigger pin change interrupt
    static void staticEncoderInterruptPortB();
    static void staticEncoderInterruptPortC();
    static void staticEncoderInterruptPortD();

    SuperRotaryEncoder(int pinA, int pinB, int pinPush);
    void setEncoderValue(int value);
    void setEncoderStep(int encstep);
    void setEncoderFastRotateTimeMs(int fastTimeMs);

    static void sleepMicroSec(unsigned long microsec);
    bool checkIfDebouncePass(int a, int b);

    void processEncoderInterrupt(int portValues);

    int getValue();
    void setMinMaxValue(int min, int max);

    int getButtonStat();
    int getButtonClicked();

};

















#endif
