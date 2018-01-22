/**
     connect temp sensor DS18B20, connect LCD
     LCD :  2-7
     DS18B20: 12
*/



#include <LiquidCrystal.h>




#define ONE_WIRE_BUS 12 /*-(Connect to Pin 2 )-*/
//LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
LiquidCrystal lcd(2, 3, 4, 5, 6, 7 );

const int IS_DEBUG = false;


void setup() {
  // put your setup code here, to run once:
  if (IS_DEBUG)
  {
  }
  lcd.begin(16, 2);
  lcd.display();
}



void loop() {

  static int i = 4;


  if (i % 4 != 0)
  {
    lcd.print(".");
    delay(300);
    i++;
    return;
  }
  i = 1;

  
  // put your main code here, to run repeatedly:
  char text[] = "A0:";
  /**
   * 
   * analogRead()
     Description
     Reads the value from the specified analog pin. The Arduino board contains a 6 channel (8 channels on the Mini and Nano, 16 on the Mega), 10-bit analog to digital converter. 
     This means that it will map input voltages between 0 and 5 volts into integer values between 0 and 1023. This yields a resolution between readings of: 5 volts / 1024 units or, .0049 volts (4.9 mV) per unit. 
     The input range and resolution can be changed using analogReference(). 
   */
  double analogReadvalue = analogRead(A0);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("raw v:");
  lcd.print(analogReadvalue );
  lcd.setCursor(0, 1);
  lcd.print(analogReadvalue*5.0/1024.0  );


  delay(300);



}


