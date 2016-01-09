/* YourDuino Electronic Brick Test
Temperature Sensor DS18B20
- Connect cable to Arduino Digital I/O Pin 2
terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <OneWire.h>
#include <DallasTemperature.h>

/*-----( Declare Constants )-----*/
#define ONE_WIRE_BUS 11 /*-(Connect to Pin 2 )-*/

/*-----( Declare objects )-----*/
/* Set up a oneWire instance to communicate with any OneWire device*/
OneWire ourWire(ONE_WIRE_BUS);

/* Tell Dallas Temperature Library to use oneWire Library */
DallasTemperature sensors(&ourWire);

/*-----( Declare Variables )-----*/


void setup() /*----( SETUP: RUNS ONCE )----*/
{
/*-(start serial port to see results )-*/
delay(1000);
Serial.begin(9600);
Serial.println("YourDuino.com: Electronic Brick Test Program");
Serial.println("Temperature Sensor DS18B20");
delay(1000);

/*-( Start up the DallasTemperature library )-*/
//sensors.setWaitForConversion();
sensors.begin();
}/*--(end setup )---*/





double tmpAnalogToDegree(double analogReading)
{
    //The voltage out is 0V at -50°C and 1.75V at 125°C. You can easily calculate the temperature from the voltage in millivolts: Temp °C = 100*(reading in V) - 50
    double temp = 0;
    double analog_V = analogReading *4.67/ 1024.0;
     temp = analog_V *100-50;

     Serial.print( analog_V);
     Serial.print(" ");
     return temp;
}



void loop() /*----( LOOP: RUNS CONSTANTLY )----*/
{
Serial.println();
Serial.print("Requesting temperature...");
sensors.requestTemperatures(); // Send the command to get temperatures
Serial.println("DONE");

Serial.print("Device 1 (index 0) = ");
Serial.print(sensors.getTempCByIndex(0));
Serial.println(" Degrees C");
//Serial.print("Device 1 (index 0) = ");
//Serial.print(sensors.getTempFByIndex(0));
//Serial.println(" Degrees F");


  double sensorValue = analogRead(A0);
  // print out the value you read:
     Serial.print( sensorValue);
     Serial.print(" ");

  double temp = tmpAnalogToDegree(sensorValue);
  Serial.println(temp);
  
delay(1000);


}/* --(end main loop )-- */

/* ( THE END ) */
