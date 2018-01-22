
#include <ar1010lib.h>
#include <Wire.h>

#define Station883 883 // FM 88.3Mhz
#define Station1053 1053 // FM 88.3Mhz

const bool debugPrint = true;


AR1010 radio = AR1010();

void setup() {

  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  if (debugPrint  ) {
    Serial.begin(9600);      // open the serial port at 9600 bps:
  }
  digitalWrite(LED_BUILTIN, HIGH);

  // soft start, wait for power to be stable
  Wire.begin();

  delay(500);


  radio.initialise();
  delay(1500);

  radio.setFrequency(Station1053 );
  delay(500);
  int freq = radio.frequency();
  Serial.print(freq);
  Serial.print("\n");
  radio.setVolume(12);
  radio.setHardmute(false);
  radio.setSoftmute(false);
}


void sleepMillieSecond(unsigned milSec)
{
   unsigned msec;
   for( msec=0;msec<milSec;msec++)
   {  
      delayMicroseconds(1000);
   }
    
}

void loop() {
  // put your main code here, to run repeatedly:

   //radio.setFrequency(Station883);
    digitalWrite(LED_BUILTIN, HIGH);
   sleepMillieSecond(700);

   //radio.setVolume(15);
   
   digitalWrite(LED_BUILTIN, LOW);
   sleepMillieSecond(700);
   //radio.seek ('u' );
   
   //sleepMillieSecond(700);
   //Serial.print("\n");
   int freq = radio.frequency();
   
   Serial.print(freq);
   Serial.print("\n");
   delay(3000);
   
   

}
