

#include "Super_TM1637_Arduino.hpp"
//for time interrupt
#include <TimerOne.h>
#include <EEPROM.h>

const int MaxSaveMinute = 60;
const unsigned long  MAXALARMSEC = 60 * 60; // max alarm timer is 60 minutes, or 3600 sec
const bool Debug = true;
const float LOWVBAT = 3.55;
const int BattCheckPin = A0;
//  handle pin change interrupt for D0 to D7 here, PIND (PortD)
//  PCINT2_vect
const int Button1_PIN = 2;
const int Button2_PIN = 3;


const int BUZZER_PIN = 6;
const int CLK_PIN = 10;
const int DIO_PIN = 11;
const int InstallDigits = 4;
Super_TM1637_Arduino myTM1637(CLK_PIN, DIO_PIN, InstallDigits);


const int buz_short_time_ms = 2;
const int buz_mid_time_ms = 200;
const int buz_long_time_ms = 2000;
const int ShortBeepEachNSecond = 5;
const unsigned long HalfSecond_MicroSec =  500000;
const unsigned long TwoSecond_MicroSec =  2000000;

const int DefaultAlarmMin = 20;
volatile long alarm_timer_sec = DefaultAlarmMin * 60; // so it's 20 minutes, or 1200 sec )

const String FinalStringZero = "0:00";

volatile bool alarmIsFired = false;

bool enableBuzzer = false;

bool timerIsStarted = false;

int StartTimerAfterNSecond = 10;
bool saveSettingEnabled = true;
const int AllowSaveWithinNSec = 10;

void buzzOn()
{
  digitalWrite(BUZZER_PIN , HIGH);
}

void buzzOff()
{
  digitalWrite(BUZZER_PIN , LOW);
}


String secToHumanMinAndSecTime(int second, bool colon = true)
{
  String minute  = String(second / 60);
  int sec = (second % 60);
  String secStr = "";
  if (sec <= 9 )
  {
    secStr = "0" + String(sec);
  } else
  {
    secStr = String(sec);
  }

  const char TimeSeparatorSymbol = ':';
  String humantime;
  if (colon )
  {
    humantime = minute + TimeSeparatorSymbol + secStr;
  } else
  {
    humantime = minute + secStr;
  }
  return humantime;
}


void pciSetup(byte pin)
{
  // enable pin
  *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));

  // clear any outstanding interrupt
  PCIFR  |= bit (digitalPinToPCICRbit(pin));

  // enable interrupt for the group
  PCICR  |= bit (digitalPinToPCICRbit(pin));
}


void saveAlarmSetting()
{
  // save alarm_timer_sec
  // EEPROM.write(0,alarm_timer_sec );
  if ( saveSettingEnabled)
  {
    int saveMin = alarm_timer_sec / 60;
    if ( alarm_timer_sec % 60 != 0)
    {
      // if 1:01 to 1:59, save as 2.
      saveMin += 1;
    }
    if ( saveMin <= 0)
    {
      // min save value, 1 minute
      saveMin = 1;
    }
    Serial.print("eeprom save");
    Serial.println(saveMin);
    //EEPROM.put(0, saveMin  );
    EEPROM.write(0, saveMin);
    buzzOn();
    delay(buz_short_time_ms);
    buzzOff();

  }

}
void loadAlarmSetting()
{
  // load alarm_timer_sec
  int loadMin = 0;
  //EEPROM.get(0, loadMin );
  loadMin = EEPROM.read( 0 );
  if (loadMin <= 0)
  {
    loadMin = 1;
  } else if (loadMin >= 255)
  {
    loadMin = DefaultAlarmMin;
  }

  alarm_timer_sec = loadMin * 60;
  if ( alarm_timer_sec >= MAXALARMSEC )
  {
    alarm_timer_sec = MAXALARMSEC ;
  }
  if (Debug)
  {
    Serial.print("eeprom read minutes ");
    Serial.println(loadMin);
  }

}

void alarmTimerIncr()
{
  alarm_timer_sec += 60;
  if ( alarm_timer_sec > MAXALARMSEC)
  {
    alarm_timer_sec = MAXALARMSEC;
  }

  alarmIsFired = false;
  //buzzOff();

}

void alarmTimerDecr()
{
  alarm_timer_sec -= 60;
  if (alarm_timer_sec <= 0)
  {
    alarm_timer_sec = 0;
  }


}


static void button1Pressed()
{

  if (Debug)
  {
    Serial.println("Button1");
  }
  alarmTimerIncr();

}

static void button2Pressed()
{
  if (Debug)
  {
    Serial.println("Button2");
  }
  alarmTimerDecr();
}


void displayHumanTimeFlashingColon(unsigned long second, bool showColon = true)
{
  String humanTime;
  humanTime = secToHumanMinAndSecTime(second, showColon);

  myTM1637.display(humanTime);
}

// handle pin change interrupt for D0 to D7 here, PIND (PortD)
static void staticButtonInterruptPortD()
{
  static int lastValue1 = LOW;
  static int lastValue2 = LOW;


  int value1 = digitalRead( Button1_PIN );
  int value2 = digitalRead( Button2_PIN );
  bool buttonPressed = false;

  if (value1 != lastValue1 )
  {
    lastValue1 = value1;
    if ( value1 == LOW)
    {
      buttonPressed = true;
      buzzOn();
      button1Pressed();

    } else
    {
      buzzOff();
    }
  } else if (value2 != lastValue2 )
  {
    lastValue2 = value2;
    if ( value2 == LOW)
    {
      buttonPressed = true;
      buzzOn();
      button2Pressed();
    } else
    {
      buzzOff();
    }
  }

  if ( buttonPressed)
  {
    saveAlarmSetting();
    displayHumanTimeFlashingColon(alarm_timer_sec);
  }


}
// handle pin change interrupt for D8 to D13 here, PINB (PortB)
ISR(PCINT2_vect)
{
  staticButtonInterruptPortD();
}



void fireAlarm()
{ 
  const int ONOFFPWM=3; // 1,2,3:ON  , 4:OFF 
  static int counter = 0;
  int alarmOn = counter%ONOFFPWM ;
  
  if( alarmOn)
  {
     buzzOn();
  }else
  {
    buzzOff();
  }
  
  if( counter%2)
  { 
    myTM1637.display(FinalStringZero);
  }
  counter ++;
}


void alarmCountingInterrupt()
{
  volatile static bool halfSec = false;
  halfSec = !halfSec ;
  if ( alarm_timer_sec <= 0 )
  {
    // alarm timer is done.
    // now init the buzzer timer.
    alarmIsFired = true;

    
  } else
  {


    if (halfSec )
    {

      if (Debug)
      {
        Serial.println(" half sec " + String(alarm_timer_sec));
      }
    } else
    {
      if (Debug)
      {
        Serial.println(" about to -- " + String(alarm_timer_sec));
      }
      alarm_timer_sec --;


    }
    displayHumanTimeFlashingColon( alarm_timer_sec, !halfSec );

  }
}
 

// before alarm is fired, timer is counting
void alarmTimerInterrupt()          // timer compare interrupt service routine
{

  if ( !alarmIsFired )
  {
    alarmCountingInterrupt();
  }

}

// after alarm is fired, use this one to control the buzzer.
void buzzerTimerInterrupt()
{
  static bool toggleBuzz = false;
  if (toggleBuzz)
  {
    buzzOn();
  }
  else
  {
    buzzOff();
  }

}

void setupAlarmTimerInt()
{
  Timer1.stop();
  Timer1.initialize(HalfSecond_MicroSec);
  Timer1.attachInterrupt( alarmTimerInterrupt );
  //Timer1.stop();
  Timer1.start();
  timerIsStarted = true;


}

void setup() {
  // put your setup code here, to run once:

  if ( Debug )
  {
    Serial.begin(115200);
  }
  pinMode(BattCheckPin, INPUT);

  alarmIsFired = false;
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN , LOW);

  pinMode(Button1_PIN, INPUT_PULLUP);
  pinMode(Button2_PIN, INPUT_PULLUP);
  pciSetup(Button1_PIN);
  pciSetup(Button2_PIN);

  myTM1637.setBrightness(2);
  loadAlarmSetting();
  displayHumanTimeFlashingColon( alarm_timer_sec );

  if (Debug)
  {
    Serial.println("=============== setup ===============   " );
  }


}

void lowBattWarning(float vbat)
{
  const String LowVBatMsg = "LoBA";

  int Loop = 3;
  for (int i = 0; i < Loop; i++)
  {
    buzzOn();
    myTM1637.display(LowVBatMsg);
    delay(1000);
    myTM1637.display(vbat);
    buzzOff();
    delay(800);

  }
}


int analogReadAvgVBatt()
{
   const int AvgMax = 6;
   int batReadInt = 0;
   for(int i=0;i< AvgMax;i++)
   {
      delay(20);
      batReadInt = batReadInt + analogRead(BattCheckPin);
   }
   batReadInt /= AvgMax;
   return batReadInt;
}

void checkVBatt()
{
  // just in case.
  buzzOff();
  delay(50);
  int vbatInt = analogReadAvgVBatt();
  
  const float VCC5 = 4.94f;
  const int ADCMAPMAX1023 = 1023;
  if (Debug)
  {
    Serial.print("batRead voltage: ");
    Serial.println(vbatInt);
  }
  //cannot use map, it's for int.
  float vbat =   vbatInt * 1.0f * VCC5 / ADCMAPMAX1023;

  if (Debug)
  {
    Serial.print("vbat");
    Serial.println(vbat );
  }

  


  if (vbat < LOWVBAT)
  {
    lowBattWarning(vbat);
  }

}


void displayFlashAlarmSetting()
{
  // show Alarm Setting For N Sec: StartTimerAfterNSecond
  for (int i = 0; i < StartTimerAfterNSecond; i++)
  {
    displayHumanTimeFlashingColon(alarm_timer_sec);
    delay(800);
    myTM1637.display("");
    delay(200);
  }
}

void beforeTimerStart()
{
  if (Debug)
  {
    Serial.println("delay " + String(StartTimerAfterNSecond) + "sec"  );
  }
  delay(300);
  checkVBatt();

  displayFlashAlarmSetting();

  buzzOn();
  delay(buz_mid_time_ms);
  buzzOff();
  setupAlarmTimerInt();
}


void loop()
{
  const int LoopInterval = 500;
  //before timer /alarm started
  if ( !timerIsStarted )
  {
    if (Debug)
    {
      Serial.println("=============== start ===============   " );
    }
    beforeTimerStart();
  }
  else
  { 
    // alarm is still counting
    if (!alarmIsFired)
    {
      int doShortBeep = !(alarm_timer_sec % ShortBeepEachNSecond );
      if (Debug)
      {
        Serial.print("short beep:");
        Serial.println(doShortBeep);
      }

      if ( doShortBeep )
      {
        buzzOn();
        delay(buz_short_time_ms);        
        buzzOff();
        delay(LoopInterval);
      }
      
      // before N second, change of the alarm timer is saved in eeprom.
      static int saveEnableCount = 0;
      if (saveSettingEnabled)
      {
        saveEnableCount++;
        if ( saveEnableCount >= AllowSaveWithinNSec*1.0f*1000/LoopInterval) 
        {
          saveSettingEnabled = false;
        }
      }

    } else
    {
      // alarm is fired already
      fireAlarm();
      
    }
  }

  //displayHumanTimeFlashingColon(alarm_timer_sec);
  delay(LoopInterval);

}
