#include "TM1637_Arduino_Chris.hpp"


// Pins connect to arduino
// if use arduino RX, TX (0,1), no pull up resistor is needed, other pins, need pull up resistor for each.
#define pin_CLK 2
#define pin_DIO 12
// if frozen, try to add two 100pf capacitors for each wire

const bool debugI2C = false;
const bool debugTM1637 = true;



TM1637_Arduino_Chris myTM1637(pin_CLK, pin_DIO);

void setup()
{
  myTM1637.m_debugPrint = debugTM1637;

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  if (myTM1637.m_debugPrint  ) {
    Serial.begin(9600);      // open the serial port at 9600 bps:
  }
  digitalWrite(LED_BUILTIN, HIGH);
  // myTM1637.showNumber(10);

  myTM1637.clearAll();

    myTM1637.doTest();

}


void loop()
{
  static unsigned long i = 0;
  digitalWrite(LED_BUILTIN, HIGH);
  // i++;
  //myTM1637.showNumber(10);
  // myTM1637.showNumber("   123456789");
  //delay(1200);
  //myTM1637.showNumber("123456");
  delay(1200);

  digitalWrite(LED_BUILTIN, LOW);
}


