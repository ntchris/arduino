// Important Note, receiver only works with 4.5V or above, better use 5V.
// with 4.1v (Lithium battery) it won't work!!!! so must use boost converter

#include <RH_ASK.h>
#include <SPI.h>
#include "LowPower.h"

//RH_ASK rf_driver;

const bool DebugPrint = false;

const int rxPin=11;
const int txPin=12;
const int pttPin=10;
RH_ASK rf_driver(1000,rxPin,txPin,pttPin,false);


const int Buzzer_Pin = 5;
const int Internal_LED = 13;
const int Buff_MAX = 5;
const int BuzzerOnDurationMillSec = 30;
const int BuzzerOffDelayMillSec = 5 * 1000; // buzzer only sound once per 20sec
unsigned long KeepReceivingDurationMs = 2500;

// since already received, so wait for some time to receive again.

const period_t  DeepSleepReceiveNothingDeepSleepPeriod = SLEEP_8S;

const String DoorOpenKeyword = "o";
const String DoorCloseKeyword = "c";

// wait 3 minutes before sound the alarm
const unsigned long Alarm_Delay_After_Open_MillSec = 1 * 1000;// 3*60*1000
// when the door is open?
unsigned long timestampDoorOpen = 0;

// this is NOT millisec!!!!
period_t deepSleepPeriod = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(Internal_LED, OUTPUT);
  pinMode(rxPin, INPUT);
  pinMode(pttPin, OUTPUT);
  pinMode(Buzzer_Pin, OUTPUT);
  buzzerOFF();

  rf_driver.init();
  if (DebugPrint)
  {
    Serial.begin(9600);
  }
}


void buzzerON()
{
  digitalWrite(Buzzer_Pin, LOW);
}

void buzzerOFF()
{
  digitalWrite(Buzzer_Pin, HIGH);
}

void ledON()
{
    digitalWrite(Internal_LED, HIGH);

}


void ledOFF()
{
    digitalWrite(Internal_LED, LOW);

}


void process_alarm()
{
  if (timestampDoorOpen == 0)
  {
    // this is the first time door open
    timestampDoorOpen = millis();
  } else
  {
    unsigned long now = millis();
    //if ((now - timestampDoorOpen) > Alarm_Delay_After_Open_MillSec)
    {
      //static int alarmOn=false;
      //alarmOn=true;
      buzzerON();
      delay(BuzzerOnDurationMillSec);
      buzzerOFF();
    }
  }
}

// keep trying to receive for duration
// return immediately and return timestamp if receive anything
String repeatReceive(unsigned long duration)
{
  unsigned long startingTS = millis();
  uint8_t available_len = Buff_MAX;
  uint8_t buf[Buff_MAX];
  memset(buf, 0, Buff_MAX );
  do {
    unsigned long now = millis();
    int r = rf_driver.recv(buf, &available_len);
    if (r > 0)
    {
      if (DebugPrint)
      {
        Serial.println("message Received:" + String(available_len));
        Serial.println((char*)buf);
        delay(100);
      }
    }

    if (r)
    { //receive something, good.
      break;
    }
    if ( (now - startingTS) > duration )
    { // too long, give up
      break;
    }
    Serial.println("retry");

  } while (true);
  String str = buf;
  return str;
}

void enable433MhzRadioReceiver()
{
  digitalWrite(pttPin, HIGH);  
}

void disable433MhzRadioReceiver()
{
  digitalWrite(pttPin, LOW);  
}

void receiveString()
{
  digitalWrite(Internal_LED, LOW);
  enable433MhzRadioReceiver();
  
  // len here is extremely important, must larger than the expected msg length
  String recvString = repeatReceive(KeepReceivingDurationMs);
  disable433MhzRadioReceiver();

  if (DoorOpenKeyword == recvString)
  {    
    ledON();
    //
    //process_alarm();
    buzzerON();
    delay(BuzzerOnDurationMillSec);
    buzzerOFF();
    ledOFF();

  }
  else if (DoorCloseKeyword == recvString)
  {
    //
  }
  else // received nothing
  {
    static unsigned long count = 0;
    //Serial.println(count );
    timestampDoorOpen = 0;
    //received nothing, rest a while
    //delay(ReceiveNothingRestDelayMillSec);
    deepSleepPeriod = DeepSleepReceiveNothingDeepSleepPeriod;
    // for serial print only, ignore in normal case
    if (DebugPrint)
    {
      Serial.println("received nothing");
      delay(100);
    }
  }
}
 
void loop()
{
  // do it once and sleep 16 seconds
  receiveString();
  // 8S
  LowPower.powerDown( DeepSleepReceiveNothingDeepSleepPeriod, ADC_OFF, BOD_OFF);
  // 8S
  LowPower.powerDown( SLEEP_2S, ADC_OFF, BOD_OFF);
}
