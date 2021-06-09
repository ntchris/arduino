

#include "Super_TM1637_Arduino.hpp"
//for time interrupt
#include <TimerOne.h>
#include <EEPROM.h>

const unsigned long  MAXALARMSEC = 60 * 60; // max alarm timer is 60 minutes, or 3600 sec
const bool Debug = true;
const float LOWVBAT = 3.58;
const int BattCheckPin = A0;
//  handle pin change interrupt for D0 to D7 here, PIND (PortD)
//  PCINT2_vect
const int Button1_PIN = 3;
const int Button2_PIN = 2;


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

volatile long alarm_timer_sec = 5; // so it's 20 minutes, or 1200 sec )

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
    int saveMin = alarm_timer_sec/60;
    if ( alarm_timer_sec%60 !=0)
    {
      // if 1:01 to 1:59, save as 2.
      saveMin +=1;
    }
    if ( saveMin <=0)
    {
      // min save value, 1 minute
      saveMin = 1;
    }
    Serial.print("eeprom save");
    Serial.println(saveMin);
    EEPROM.put(0, saveMin  );
  }

}
void loadAlarmSetting()
{
  // load alarm_timer_sec
  int loadMin=0;
  EEPROM.get(0, loadMin );
  alarm_timer_sec= loadMin * 60;
  if( alarm_timer_sec >= MAXALARMSEC )
  {
    alarm_timer_sec = MAXALARMSEC ;
  }
  if (Debug)
  {
    Serial.print("eeprom read");
    Serial.println(alarm_timer_sec);
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

  if( buttonPressed)
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
  //buzzOn();
  myTM1637.display(FinalStringZero);

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

    fireAlarm();
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

void timeOutAlarmInterrupt()
{

}

// before alarm is fired, timer is counting
void alarmTimerInterrupt()          // timer compare interrupt service routine
{

  if (Debug)
  { Serial.print("in int");
  }
  if ( !alarmIsFired )
  {
    alarmCountingInterrupt();
  } else
  {
    timeOutAlarmInterrupt();
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

  myTM1637.setBrightness(buz_short_time_ms);
  loadAlarmSetting();
  displayHumanTimeFlashingColon( alarm_timer_sec );

  if (Debug)
  {
    Serial.println("=============== setup ===============   " );
  }


}

void checkVBatt()
{
  int batRead = analogRead(BattCheckPin);
  const int VCC5 = 5;
  const int ADCMAPMAX1023 = 1023;
  if (Debug)
  {
    Serial.print("batRead");
    Serial.println(batRead);
  }
  //cannot use map, it's for int.
  float vbat =   batRead * 1.0f * VCC5 / ADCMAPMAX1023;

  if (Debug)
  {
    Serial.print("vbat");
    Serial.println(vbat );
  }

  const String LowVBatMsg = "LoBA";
  if (vbat < LOWVBAT)
  {
    buzzOn();
    myTM1637.display(LowVBatMsg);
    delay(1500);
    buzzOff();
    myTM1637.display("");
    delay(500);
    myTM1637.display(LowVBatMsg);
    delay(500);
    buzzOn();
    delay(1500);
    buzzOff();
  }

}


void displayFlashAlarmSetting()
{
  int showAlarmSettingForNSec = StartTimerAfterNSecond;
  for (int i = 0; i < showAlarmSettingForNSec; i++)
  {
    displayHumanTimeFlashingColon(alarm_timer_sec);
    delay(700);
    myTM1637.display("");
    delay(300);
  }
}

void beforeTimerStart()
{
  if (Debug)
  {
    Serial.println("delay " + String(StartTimerAfterNSecond) + "sec"  );
  }
  delay(300);
  //checkVBatt();
  
  displayFlashAlarmSetting();

  buzzOn();
  delay(buz_mid_time_ms);
  buzzOff();
  setupAlarmTimerInt();
}


void loopzzz()
{
  static int loopCount = 0;
  loopCount ++;
  if (Debug)
  {
    Serial.println("alarm_timer_sec " + String(alarm_timer_sec ) );
  }
  delay(2000);
}

void loop()
{

  if ( !timerIsStarted )
  {

    if (Debug)
    {
      Serial.println("=============== start ===============   " );
    }
    beforeTimerStart();
  }

  if (timerIsStarted)
  {
    if (!alarmIsFired)
    {
      int testShortBeep = alarm_timer_sec % ShortBeepEachNSecond ;
      if (Debug)
      {
        Serial.print("short beep:");
        Serial.println(testShortBeep);
      }

      if ( testShortBeep == 0 )
      {
        digitalWrite(BUZZER_PIN , HIGH);
        delay(buz_short_time_ms);
        digitalWrite(BUZZER_PIN , LOW);
      }

      static int saveEnableCount = 0;
      if (saveSettingEnabled)
      {
        saveEnableCount++;
        if ( saveEnableCount >= AllowSaveWithinNSec )
        {
          saveSettingEnabled = false;
        }
      }

    } else
    {
      // alarm is fired already

    }
  }

  //displayHumanTimeFlashingColon(alarm_timer_sec);
  delay(1000);

}
