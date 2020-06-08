#include "superRotaryEncoder.hpp"


static int SuperRotaryEncoder::_rotaryEncoderA,  SuperRotaryEncoder::_rotaryEncoderB;
static bool SuperRotaryEncoder::debug = true;
static int SuperRotaryEncoder::encoderValue = 100;

ISR (PCINT0_vect) // handle pin change interrupt for D8 to D13 here
{
  //digitalWrite(13,digitalRead(8) and digitalRead(9));
  SuperRotaryEncoder::processEncoderInterrupt();
  //Serial.println("!!!");
}

void pciSetup(byte pin)
{
  *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
  PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
  PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}


int SuperRotaryEncoder::getValue()
{
  return encoderValue;
}

SuperRotaryEncoder::SuperRotaryEncoder(int pinA, int pinB)
{
  _rotaryEncoderA = pinA;
  _rotaryEncoderB = pinB;
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);

  pciSetup(pinA);
  // only necessary to active pin A interrupt ??!
  //pciSetup(pinB);
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
  int valueADeb = digitalRead( _rotaryEncoderA );
  int valueBDeb = digitalRead( _rotaryEncoderB );
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

  int valueA = LOW;
  int valueB = LOW;
  static int lastValueA = LOW;
  static int lastValueB = LOW;
  static int dir = 0; // >0 : Right,   <0 : Left
  valueA = digitalRead( _rotaryEncoderA );
  valueB = digitalRead( _rotaryEncoderB );

  bool pass = checkIfDebouncePass(valueA, valueB);
  if (!pass)
  {
    //fail debounce check
    return;
  }
  /*
    if (valueA == lastValueA)
    {
    if (SuperRotaryEncoder::debug)
    {
      Serial.print("valueA ");
      Serial.println(valueA);
      Serial.println("thisA == lastA");
      Serial.print("thisB ");
      Serial.println(valueB);
      Serial.print("lastB ");
      Serial.println(lastValueB);
    }
    //why are we here ?
    // only assummmeee this is rising

    return;
    }
  */
  if (lastValueA == LOW && valueA == HIGH )
  {
    //A is rising
    if (SuperRotaryEncoder::debug)
    {
      Serial.println("A rising");
    }

    if (lastValueB == HIGH && valueB == HIGH)
    {
      Serial.println("dir++");
      dir++;
    }
    else if (lastValueB == LOW && valueB == LOW)
    {
      Serial.println("dir--");
      // ==================== check for left =======================
      dir--;
    } else if (valueB == HIGH)
    {
      Serial.println("dir++");
      dir++;

      /*Serial.print("lastB ");
        Serial.print(lastValueB);
        Serial.print(" B ");
        Serial.println(valueB);
      */

    }
  } else if (lastValueA == HIGH && valueA == LOW )
  { // A is falling
    Serial.println("A falling");
    if ( valueB == LOW)
    {
      Serial.println("dir++");

      dir++;
    } else
    {
      Serial.println("dir--");
      dir--;
    }
  } else if (lastValueA == HIGH && valueA == HIGH)
  {
    if (SuperRotaryEncoder::debug)
    {
      Serial.println("what to do for A H H?");
    }
  } else if (lastValueA == LOW && valueA == LOW)
  {
    if (SuperRotaryEncoder::debug)
    {
      Serial.println("what to do for A L L?");
    }
  }

  if (dir >= 2)
  {
    Serial.println("++ and set lastDir is 0");
    encoderValue ++;
    dir = 0;
  } else if (dir <= -2)
  {
    Serial.println("-- and set lastDir is 0");
    encoderValue --;
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
  int valueA = digitalRead( _rotaryEncoderA   );
  int valueB = digitalRead( _rotaryEncoderB   );

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
