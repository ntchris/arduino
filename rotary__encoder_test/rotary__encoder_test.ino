
#include "superRotaryEncoder.hpp"


// built in LED
const int LedPin = 13;
const int RotaryEncoderA = 11;
const int RotaryEncoderB = 10;
const int RotaryEncoderPush = 12;

static SuperRotaryEncoder rotEncoder(RotaryEncoderA, RotaryEncoderB, RotaryEncoderPush );
// Connect pin C to GND
// HOW to determine which PIN is A, which is B, but it deosn't really matter.
// Using  displayValues() function, check serial output window
// rotate left, the pin always first turn to 0 from 1, is A.  the other pin is B.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("Setup \n");

  pinMode(LedPin, OUTPUT);



  digitalWrite(LedPin, LOW);
  //rotEncoder.debug = true;
  rotEncoder.setEncoderValue(150);
  rotEncoder.setEncoderStep(2);
}



void loop() {


  static int lastVal = 0;
  static int lastButton = HIGH;
  int val = rotEncoder.getValue();
  if (val != lastVal)
  {
    lastVal = val;
    Serial.print(val);

    Serial.print("\n");

  }

  int button = rotEncoder.getButtonStat();
  if(lastButton != button )
  {
    Serial.println("button " + String(button));
    lastButton = button;
  }
  

  int clicked = rotEncoder.getButtonClicked();
  if (clicked )
  {
    Serial.println("clicked");
  }

  delay(100);


}
