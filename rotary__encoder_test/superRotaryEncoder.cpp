#include "superRotaryEncoder.hpp"


bool SuperRotaryEncoder::debug = false;

int SuperRotaryEncoder::_fastRotateMs = 50;

SuperRotaryEncoder *SuperRotaryEncoder::encoderInstance = NULL;


// handle pin change interrupt for D8 to D13 here, PINB (PortB)
ISR(PCINT0_vect)
{
  SuperRotaryEncoder::staticEncoderInterruptPortB();
}


// handle pin change interrupt for A0 to A5 here, PINC (PortC)
ISR (PCINT1_vect)
{
  SuperRotaryEncoder::staticEncoderInterruptPortC();
}  // end of PCINT1_vect


// handle pin change interrupt for D0 to D7 here, PIND (PortD)
ISR (PCINT2_vect)
{
  SuperRotaryEncoder::staticEncoderInterruptPortD();
}  // end of PCINT2_vect

void pciSetup(byte pin)
{
  // enable pin
  *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));

  // clear any outstanding interrupt
  PCIFR  |= bit (digitalPinToPCICRbit(pin));

  // enable interrupt for the group
  PCICR  |= bit (digitalPinToPCICRbit(pin));
}


void SuperRotaryEncoder::staticEncoderInterruptPortB()
{
  // get All Pin status using PINB since we are using pin D9--D12

  if (debug) {
    Serial.print("PINB ");
    Serial.println(PINB, BIN);
  }

  if ( encoderInstance )
  {
    encoderInstance->processEncoderInterrupt(PINB);
  } else
  {
    if (debug)
    {
      Serial.println("null pointer encoder no instance");
    }
  }
}


void SuperRotaryEncoder::staticEncoderInterruptPortC()
{
  if (debug)
  { Serial.print("PINC ");
    Serial.println(PINC, BIN);
  }
  if ( encoderInstance )
  {
    encoderInstance->processEncoderInterrupt(PINC);
  } else
  {
    if (debug)
    {
      Serial.println("null pointer encoder no instance");
    }
  }
}


void SuperRotaryEncoder::staticEncoderInterruptPortD()
{
  if (debug)
  { Serial.print("PIND ");
    Serial.println(PIND, BIN);
  }

  if ( encoderInstance )
  {
    encoderInstance->processEncoderInterrupt(PIND);
  } else
  {
    if (debug)
    {
      Serial.println("null pointer encoder no instance");
    }
  }
}

// constructor
SuperRotaryEncoder::SuperRotaryEncoder(int pinA, int pinB, int pinButton)
{
  _rotaryEncoderPinA = pinA;
  _rotaryEncoderPinB = pinB;
  _rotaryEncoderPinButton = pinButton;
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinButton, INPUT_PULLUP);
  encoderValue = 100;
  encStep = 1;
  // the default min and max value
  minValue = 0, maxValue = 200;
  buttonStatus = HIGH;

  //setup interrupt
  pciSetup(pinA);

  pciSetup(pinButton);

  encoderInstance = this;
  //pciSetup(pinB);  // B interrupt is NOT needed.

  // faster than 300ms is fast.
  _fastRotateMs = 50;
}


int SuperRotaryEncoder::getButtonStat()
{
  return buttonStatus;
}

int SuperRotaryEncoder::getButtonClicked()
{
  if ( isButtonClicked )
  {
    // reset click
    isButtonClicked = false;
    return true;
  }
  return false;
}

int SuperRotaryEncoder::getValue()
{
  return encoderValue;
}


void SuperRotaryEncoder::setEncoderValue(int value)
{
  encoderValue = value;
}

void SuperRotaryEncoder::setEncoderStep(int encstep)
{
  encStep = encstep;
}

void SuperRotaryEncoder::setMinMaxValue(int min, int max)
{
  if ( min >= max)
  {
    Serial.print("Error min >= max");
  }

  minValue = min;
  maxValue = max;

}

void SuperRotaryEncoder::setEncoderFastRotateTimeMs(int fastTimeMs)
{
  _fastRotateMs = fastTimeMs;
}



void SuperRotaryEncoder::processEncoderInterrupt(int portValues)
{


  if (debug) {
    Serial.print("Port ");
    Serial.println(portValues, BIN);
  }

  static int lastValueA = HIGH;
  static int lastValueB = HIGH;
  static int dir = 0; // >0 : Right,   <0 : Left
  int valueA = digitalRead( _rotaryEncoderPinA );
  int valueB = digitalRead( _rotaryEncoderPinB );

  // =========  process button ===========
  static int lastButtonStatus = HIGH;
  buttonStatus = digitalRead(_rotaryEncoderPinButton);
  if (lastButtonStatus == LOW && buttonStatus == HIGH)
  {
    isButtonClicked = true;
    if (debug) {
      Serial.println("...interrupt clicked ...");
    }

  }
  lastButtonStatus = buttonStatus;
  // ======================================


  if (lastValueA == LOW && valueA == HIGH )
  {
    //A is rising
    if (SuperRotaryEncoder::debug)
    {
      Serial.println("A rising");
    }

    if (lastValueB == HIGH )
    {
      if (debug) {
        Serial.println("dir++");
      }
      dir++;
    }
    else if (lastValueB == LOW )
    {
      if (debug) {
        Serial.println("dir--");
      }
      // ==================== check for left =======================
      dir--;
    }

  } else if (lastValueA == HIGH && valueA == LOW )
  { // A is falling
    if (debug) {
      Serial.println("A falling");
    }
    if ( valueB == HIGH )
    {
      if (debug) {
        Serial.println("dir++");
      }

      dir++;
    } else if ( valueB == LOW )
    {
      if (debug) {
        Serial.println("dir--");
      }
      dir--;
    }
  } else
  {
    if (debug)
    { Serial.print("Interrupt but A no change???");
      Serial.print("lastA ");
      Serial.print(lastValueA);
      Serial.print(" A ");
      Serial.println(valueA);
    }
  }
  /*else if (lastValueB == LOW && valueA == HIGH)
    {
    // B is rising
    if (SuperRotaryEncoder::debug)
    {
      Serial.println("what to do for A H H?");
    }
    } else if (lastValueB == HIGH && valueB == LOW)
    {
    // B is falling
    if (SuperRotaryEncoder::debug)
    {
      Serial.println("what to do for A L L?");
    }
    }*/

  static unsigned long lastTs = 0;
  unsigned long nowTs = 1000;
  unsigned long deltaMs ;
  if ( dir >= 2 || dir <= -2)
  {
    nowTs = millis();
    deltaMs =  nowTs - lastTs;
    lastTs = nowTs;
  }
  if (dir >= 2)
  {
    if (debug) {
      Serial.println("++ and set lastDir is 0");
    }
    if ( deltaMs <  _fastRotateMs)
    {
      //very fast
      Serial.println("fast!!++" + String(deltaMs));

      encoderValue += encStep * FastStepFactor;

    } else
    {
      // normal speed rotation
      encoderValue += encStep;
    }
    if ( encoderValue > maxValue )
    {
      encoderValue = maxValue;
    }

    dir = 0;

  } else if (dir <= -2)
  {
    if (debug) {
      Serial.println("-- and set lastDir is 0");
    }

    if ( deltaMs <  _fastRotateMs)
    {
      //very fast
      Serial.println("fast!!--" + String(deltaMs));

      encoderValue -= encStep * FastStepFactor;

    } else
    {
      // normal speed rotation
      encoderValue -= encStep;
    }

    if ( encoderValue < minValue)
    {
      encoderValue = minValue;
    }

    dir = 0;
  }
  lastValueA = valueA;
  lastValueB = valueB;
 
  return;

}
