// power:  2.80v not working
//         2.90v 7ma  --> 15ma working
//         3.5v     9ma ---14ma
//         4.2v    12ma ---19ma
//         5.0v    17ma ---25ma

#include <RH_ASK.h>
#include <SPI.h>
#include "LowPower.h"

//RH_ASK::RH_ASK   (   uint16_t    speed = 2000,
    //uint8_t   rxPin = 11,
    //uint8_t   txPin = 12,
    //uint8_t   pttPin = 10,
    //bool    pttInverted = false 
  //)     
  
RH_ASK rf_driver(1000);

const char *Msg = "o"; //open
bool DebugPrint = true;
int data_out = 12;
int internal_LED = 13;
int data_read = 11;

int sleepTimerMilliSec=2000;
int RepeatSend=30;

void setup() {

  rf_driver.init();
  pinMode( internal_LED, OUTPUT); //
  //pinMode( data_out, OUTPUT); //
  
  if( DebugPrint)
  {
    Serial.begin(9600);
  }

}

 

void send_string()
{
  digitalWrite(internal_LED, HIGH);
  
  int len = strlen(Msg);
  int r = rf_driver.send((uint8_t *)Msg, len);
  
  rf_driver.waitPacketSent();
  
  digitalWrite(internal_LED, LOW);
  

}

void loop()
{
  int i=0;
  for(i=0;i<RepeatSend;i++)
  { 
    send_string();
    delay(5);
  }
  LowPower.powerDown( SLEEP_2S, ADC_OFF, BOD_OFF);


}
