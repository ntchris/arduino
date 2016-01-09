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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sensors.begin();
  lcd.begin(16, 2);
    lcd.display();
}

float readTempDS18B20()
{
   sensors.requestTemperatures(); // Send the command to get temperatures
   float temp = sensors.getTempCByIndex(0);
   Serial.print(temp);
   Serial.println(" C");
   return temp;
}


void loop() {

    static int i=4;


  if(i%4!=0)
  {
       lcd.print(".");
       delay(500);
       i++;
       return;
    } 
   i=1;
    
  // put your main code here, to run repeatedly:
   float tempC = readTempDS18B20();
   lcd.clear();
   lcd.print(tempC,1);
   lcd.print('C');
   
   delay(500);
     


}


