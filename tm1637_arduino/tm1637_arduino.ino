#include "TM1637_Arduino_Chris.hpp"


// Pins connect to arduino
// if use arduino RX, TX (0,1), no pull up resistor is needed, other pins, need pull up resistor for each.
#define pin_CLK 2
#define pin_DIO 12
// if frozen, try to add two 100pf capacitors for each wire

const bool debugLowLevel = false;
const bool debugHighLevel = false;



TM1637_Arduino_Chris myTM1637(pin_CLK, pin_DIO);

void setup()
{
  myTM1637.m_debugLowLevel=debugLowLevel;
  myTM1637.m_debugHighLevel=debugHighLevel;
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  if (myTM1637.m_debugLowLevel || myTM1637.m_debugHighLevel) {
    Serial.begin(9600);      // open the serial port at 9600 bps:
  }
  myTM1637.clearAll();
  digitalWrite(LED_BUILTIN, HIGH);

}

void loop()
{
  static unsigned long i = 0;
  digitalWrite(LED_BUILTIN, HIGH);
  i++;
  myTM1637.showNumber(i);
  digitalWrite(LED_BUILTIN, LOW);

}


