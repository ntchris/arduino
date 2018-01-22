/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 


 Hardware: (Note most of these pins can be swapped)
    Graphic LCD Pin ---------- Arduino Pin
       1-VCC       ----------------  5V
       2-GND       ----------------  GND
       3-SCE       ----------------  7
       4-RST       ----------------  6
       5-D/C       ----------------  5
       6-DN(MOSI)  ----------------  11
       7-SCLK      ----------------  13
       8-LED       - 330 Ohm res --  9

***************************************************************************/


#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "LCD_Functions.h"

//const int ledPin =  LED_BUILTIN;// the number of the LED pin
//13, so can't use it.

int ledState = LOW;             // ledState used to set the LED

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

void setup() {
    //Serial.begin(9600);
    //Serial.println(F("BME280 test"));

    lcdBegin(); // This will setup our pins, and initialize the LCD
    //updateDisplay(); // with displayMap untouched, SFE logo
    setContrast(45); // Good values range from 40-60
    //delay(2500);
  
    bool status;
    
    // default settings
    status = bme.begin();
    /*if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
      
    }
    */
   
    delayTime = 2500;
 

    //pinMode(ledPin, OUTPUT);

    delay(200); // let sensor boot up
   
    clearDisplay(WHITE);
    updateDisplay();
}


void loop() { 

    float temperature, humidity, pressure, altitude;

    temperature = bme.readTemperature();
    humidity = bme.readHumidity();
    pressure = bme.readPressure() / 100.0F;
    altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    
    //printSensorValues();
    //printValuesLCD5110();
    String tempString = String("Temp: ")+String(temperature)+"C  ";
    String humidString = "Humid: " + String(humidity)+"% ";
    String pressureString = "Press: "+ String(pressure)+"Hpa ";
    String altitudeString = " Alt: " + String(altitude)+"M ";
    String toPrint = tempString + humidString + pressureString + altitudeString; 
    //Serial.print(toPrint);
    setStr(toPrint.c_str(), 0, 0, BLACK);
    updateDisplay();



    //ledState = !ledState;
      
    // set the LED with the ledState of the variable:
    //digitalWrite(ledPin, ledState);
    delay(delayTime);

}


void printValuesLCD5110()
{
   setStr("", 0, LCD_HEIGHT-8, BLACK);
   updateDisplay();

}


void printSensorValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}
