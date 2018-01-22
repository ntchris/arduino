/**
 *   connect temp sensor DS18B20, connect LCD
 *   LCD :  2-7
 *   DS18B20: 12
 */



#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>




#define ONE_WIRE_BUS 12 /*-(Connect to Pin 2 )-*/
OneWire ourWire(ONE_WIRE_BUS);
DallasTemperature sensors(&ourWire);
//LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
LiquidCrystal lcd(2,3,4,5,6,7 );

const int IS_DEBUG = false;


void setup() {
  // put your setup code here, to run once:
  if(IS_DEBUG)
  {
    Serial.begin(9600);
    sensors.begin();
  }
  lcd.begin(16, 2);
  lcd.display();
}

float readTempDS18B20()
{
   sensors.requestTemperatures(); // Send the command to get temperatures
   float temp = sensors.getTempCByIndex(0);
   if(IS_DEBUG)
   {
      Serial.print(temp);
      Serial.println(" C");
   }
  temp = temp - 1;
  return temp;
}


void loop() {

    static int i=4;


  if(i%4!=0)
  {
       lcd.print(".");
       delay(300);
       i++;
       return;
    } 
   i=1;
    
  // put your main code here, to run repeatedly:
   char text = "Hello world!";
   lcd.clear();
   lcd.print(text,1);
   
   
   delay(300);
     


}


