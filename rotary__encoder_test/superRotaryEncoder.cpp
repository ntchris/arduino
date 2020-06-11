#include "superRotaryEncoder.hpp"


//int SuperRotaryEncoder::_rotaryEncoderPinA,  SuperRotaryEncoder::_rotaryEncoderPinB;
bool SuperRotaryEncoder::debug = false;
SuperRotaryEncoder *SuperRotaryEncoder::encoderInstance = NULL;

//int SuperRotaryEncoder::encoderValue = 100;
//int SuperRotaryEncoder::valueA = HIGH, SuperRotaryEncoder::valueB = HIGH;


ISR(PCINT0_vect) // handle pin change interrupt for D8 to D13 here
{

  //SuperRotaryEncoder::processEncoderInterrupt();
  SuperRotaryEncoder::staticEncoderInterrupt();
}


ISR (PCINT1_vect)
{
  // handle pin change interrupt for A0 to A5 here
}  // end of PCINT1_vect

ISR (PCINT2_vect)
{
  // handle pin change interrupt for D0 to D7 here
}  // end of PCINT2_vect

void pciSetup(byte pin)
{
  *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
  PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
  PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}


void SuperRotaryEncoder::staticEncoderInterrupt()
{
  if ( encoderInstance )
  {
    encoderInstance->processEncoderInterrupt();
  } else
  {
    if (debug)
    {
      Serial.println("null pointer encoder no instance");
    }
  }
}

int SuperRotaryEncoder::getValue()
{
  return encoderValue;
}

SuperRotaryEncoder::SuperRotaryEncoder(int pinA, int pinB)
{
  _rotaryEncoderPinA = pinA;
  _rotaryEncoderPinB = pinB;
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  encoderValue = 100;
  encStep = 1;

  //setup interrupt
  pciSetup(pinA);

  encoderInstance = this;
  // pciSetup(pinB);  B interrupt is NOT needed.
}


void SuperRotaryEncoder::setEncoderValue(int value)
{
  encoderValue = value;
}

void SuperRotaryEncoder::setEncoderStep(int encstep)
{
  encStep = encstep;
}
unsigned long SuperRotaryEncoder::getTimeStamp()
{
  unsigned long  ts = micros();
  return ts;

}


void SuperRotaryEncoder::sleepMicroSec(unsigned long microsec)
{
  // 1 nop = 62.5 ns, 16 nops approx 1 us
  unsigned long i;
  unsigned long j;
  unsigned long microsec2 = microsec / 10;
  if (microsec2 <= 0)
  {
    microsec2 = 1;
  }

  for ( i = 0; i < microsec2; i++)
  {
    for (j = 0; j < 21; j++)
    {
      asm volatile ("NOP");
    }
  }


}

//pass true, otherwise false
bool SuperRotaryEncoder::checkIfDebouncePass(int valueA, int valueB)
{

  // debounce 100 OK
  sleepMicroSec(80);
  int valueADeb = digitalRead( _rotaryEncoderPinA );
  int valueBDeb = digitalRead( _rotaryEncoderPinB );
  if (  valueADeb == valueA && valueBDeb == valueB )
  {
    //Serial.println("pass debounce");

    return true;
  }

  //fail debounce check, ignore
  if (debug)
  {
    Serial.println("debounce quit");
  }
  return false;

}

void SuperRotaryEncoder::processEncoderInterrupt()
{
  //store the valueA B from last time

  static int lastValueA = LOW;
  static int lastValueB = LOW;
  static int dir = 0; // >0 : Right,   <0 : Left
  int valueA = digitalRead( _rotaryEncoderPinA );
  int valueB = digitalRead( _rotaryEncoderPinB );

  /*bool pass = checkIfDebouncePass(valueA, valueB);
    if (!pass)
    {
    //fail debounce check
    return;
    }
    to debounce, add capacitor from pin A to Gnd, and from pin B to Gnd
    value from 20nf to 60nf should work well.
  */

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

  if (dir >= 2)
  {
    if (debug) {
      Serial.println("++ and set lastDir is 0");
    }
    encoderValue +=encStep;
    dir = 0;
  } else if (dir <= -2)
  {
    if (debug) {
      Serial.println("-- and set lastDir is 0");
    }
    encoderValue -=encStep;
    dir = 0;
  }
  lastValueA = valueA;
  lastValueB = valueB;

  return;

}





/*
  void SuperRotaryEncoder::processEncoderInterruptV1DontUse()
  {
  static bool working = false;
  int valueA = digitalRead( _rotaryEncoderA );
  int valueB = digitalRead( _rotaryEncoderB );


  const unsigned long  AllowTimeDeltaMicroSec = 200; // key value change only accept if it's larger than this mecond

  static int lastAValue = 0;
  static int lastBValue = 0;
  static bool interruptFor;// LastIntA B Push
  static bool lastInterruptFor = IntUnknown ; // LastIntA B Push
  static int lastDir = DirUnknown;
  static unsigned long lastTimeStampA = 0;
  //static unsigned long lastTimeStampB = 0;
  static unsigned long lastTimeStampPush = 0;

  if (working)
  {
  Serial.println("stil working");

  return;
  }


  if (SuperRotaryEncoder::debug)
  {
  Serial.println("");
  Serial.println("interrupt");
  Serial.print("valueA ");
  Serial.println(valueA);
  Serial.print("valueB ");
  Serial.println(valueB);
  }

  working = true;
  // begin with A=1 B=1 and end with A=1  B=1

  static bool started = false;
  unsigned long timestamp = getTimeStamp();

  if ( !started )
  {
  if (SuperRotaryEncoder::debug)
  {
  Serial.println("1st time");
  }
  lastAValue = valueA;
  Serial.print("set last valueA ");
  Serial.println(valueA);

  lastBValue = valueB;
  started = true;
  working = false;
  lastTimeStampA = timestamp;
  return;
  }
  if (SuperRotaryEncoder::debug) {
  Serial.println("here");
  Serial.print("lastA " );
  Serial.println(lastAValue );
  }

  unsigned long delta = timestamp - lastTimeStampA;
  if (valueA == HIGH)
  {
  if (delta < AllowTimeDeltaMicroSec )
  {
  // this is bouncing, ignore
  //
  if (SuperRotaryEncoder::debug)
  {
  Serial.print("A debounce ignore ");
  Serial.print(delta);
  Serial.println("");
  }
  }
  working = false;
  return;

  }

  lastTimeStampA = timestamp;
  if ( valueA != lastAValue )
  {
  // this is interrupt for A
  interruptFor = IntA ;
  // anti bounce

  } else
  {
  // this A = lastA
  working = false;
  if (SuperRotaryEncoder::debug)
  {
  Serial.print("do nothing, A==lastA");
  }
  lastBValue = valueB;

  return;
  }

  int thisDir = DirUnknown;
  if (interruptFor == IntA)
  {
  if ( valueA == HIGH)
  {
  // so lastA must be low ??!!
  // when A is rising, b is already H, it's turning right
  if ( valueB == HIGH)
  {

  if (lastDir == DirRight )
  {
  // should +++
  if (SuperRotaryEncoder::debug) {
   Serial.println("++++++++ and set lastDir is unknown");
  }
  lastDir = DirUnknown;
  encoderValue ++;
  } else
  {
  if (SuperRotaryEncoder::debug) {
   lastDir = DirRight;
   Serial.println("but last is not right");
  }
  }

  }
  lastAValue = valueA;
  lastBValue = valueB;
  if (SuperRotaryEncoder::debug) {
  Serial.print("set last valueA H ");
  Serial.println(valueA);
  }
  lastInterruptFor = IntA;
  working = false;
  return;

  } else //  valueA==LOW
  {

  // so last A is high, and A is falling
  if (valueB == LOW && lastBValue == LOW)
  {

  lastDir = DirRight;
  if (SuperRotaryEncoder::debug) {
  Serial.println("set right part1");
  }

  } else if (valueB != lastBValue)
  {
  // cancel the first half
  //thisDir = DirUnknown;
  }

  lastAValue = valueA;
  lastBValue = valueB;
  if (SuperRotaryEncoder::debug) {
  Serial.print("set last valueA L ");
  Serial.println(valueA);
  }
  lastInterruptFor = IntA;
  working = false;
  return;

  }

  if (SuperRotaryEncoder::debug) {
  Serial.println("quit");
  }

  working = false;
  return;
  } // end of  if (interruptFor == IntA)

  }

  /*
  if ( thisDir == lastDir)
  {
  if (thisDir == DirRight)
  {
  Serial.println("++++++++ and set lastDir is unknown");
  lastDir = DirUnknown;
  encoderValue ++;
  }

  } else
  {
  lastDir = thisDir;
  Serial.println("set lastDir is right");
  }
  working = false;
  Serial.println("quit");
  return;
  }

*/








void SuperRotaryEncoder::displayValues()
{
  int valueA = digitalRead( _rotaryEncoderPinA   );
  int valueB = digitalRead( _rotaryEncoderPinB   );

  if ((valueA == HIGH) && (valueB == HIGH))
  {
    return;
  }
  Serial.println("A=" + String(valueA));

  Serial.println("B=" + String(valueB));

  Serial.println("==========");


}



void SuperRotaryEncoder::selftest()
{
  //  this->debug = true;
  while (true)
  {
    // this->getEncoderValueSum();
    //          this->getEncoderValueSumAccel();

    int value = getValue();
    Serial.println(value);
  };


}
