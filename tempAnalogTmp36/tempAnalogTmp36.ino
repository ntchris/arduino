/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

 

// the setup routine runs once when you press reset:
void setup() {
 

  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  Serial.println(sensorMin);
  Serial.println(sensorMax);
  
}

double tmpAnalogToDegree(double analogReading)
{
    //The voltage out is 0V at -50°C and 1.75V at 125°C. You can easily calculate the temperature from the voltage in millivolts: Temp °C = 100*(reading in V) - 50
    double temp = 0;
    double analog_V = analogReading *4.67/ 1024.0;
     temp = analog_V *100-50;

     Serial.println( analog_V);

     return temp;
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:

       Serial.println( "============================");
  
  double sensorValue = analogRead(A0);
  // print out the value you read:
     Serial.println( sensorValue);

  double temp = tmpAnalogToDegree(sensorValue);
  Serial.println(temp);
  delay(2000);        // delay in between reads for stability
}
