#include "Super_TM1637_Arduino.hpp"
#include <TimerOne.h>


// Pins connect to arduino
// if use arduino RX, TX (0,1), no pull up resistor is needed, other pins, need pull up resistor for each.
#define pin_CLK A1
#define pin_DIO A0
#define pin_firealarm A2
#define pin_relay_delayOff A3

// if frozen, try to add two 100pf capacitors for each wire

// only connected a 4 digit digitube
#define installDigits  4


const bool debugTM1637 = true;

const long OneSecond = 1000000;


Super_TM1637_Arduino myTM1637(pin_CLK, pin_DIO, installDigits);

//make the longest timer as one hour -1 second
const long MAXTimerSecond = 60 * 60 - 1;
const long STEP_TIME_SETTING = 30;
static long second = 0;

/*
    inline updateSecond()
    {
    second ++;
    if (second >= 60)
    {
        minute++;
    }

    if (minute >= 59)
    {
        minute = 59;
    }
    }

*/


inline void refreshDisplayTime(long second)
{
    String timestring = secondToTime(second);
    myTM1637.display(timestring);
}


void fireAlarmSound(void)
{
    digitalWrite(pin_firealarm, HIGH);

}

void stopAlarmSound(void)
{
    digitalWrite(pin_firealarm, LOW);

}


void timerInterrupt()          // timer compare interrupt service routine
{
    static int MaxAlarmDuration = 2;
    static int alarmTimeCountSecond = 0;
    if ( second == 0 )
    {

        if (alarmTimeCountSecond == 0 )
        {

            digitalWrite(pin_relay_delayOff, LOW);
            fireAlarmSound();
            alarmTimeCountSecond++;
            Serial.print("alarm fired!!! \n");
            refreshDisplayTime(second);
            return;
        }
        else if (alarmTimeCountSecond > 0 && alarmTimeCountSecond < MaxAlarmDuration  )
        {
            alarmTimeCountSecond ++;

            Serial.print("alarm fired and waiting for next time int\n");
            return;

        } else if (alarmTimeCountSecond >= MaxAlarmDuration)
        {
            alarmTimeCountSecond = 0;
            //Timer1.detachInterrupt();
            Timer1.stop();
            stopAlarmSound();
            Serial.print("alarm stoped\n");
        }

        return;
    } else
    {
        second--;
        refreshDisplayTime(second);
    }
    //should put this in another interrupt!


}

// from second to string of minute:second
inline String secondToTime(long second)
{
    int minute = second / 60;
    int displaysecond = second % 60;

    String secondstring = String(displaysecond);
    //must pad a 0 for 0 to 9.   so we have 01, 02 , 03...09.
    if (displaysecond <= 9)
    {
        secondstring = "0" + secondstring;
    }
    return String(minute) + ":" + secondstring;
}


void setup()
{
    Serial.begin(9600);
    Serial.print("Setup \n");
    myTM1637.m_debugPrint = debugTM1637;
    pinMode(pin_relay_delayOff, OUTPUT);
    digitalWrite(pin_relay_delayOff, LOW);

    pinMode(pin_firealarm , OUTPUT);
    fireAlarmSound();

    myTM1637.setBrightness(0);

    myTM1637.clearAll();
    Timer1.stop();
    //Timer1.attachInterrupt( timerInterrupt ); // attach the service routine here

    delay(100);
    stopAlarmSound();

}


void timerBottonPlus()
{
    second += STEP_TIME_SETTING;
    if (second > MAXTimerSecond )
    {
        second = MAXTimerSecond ;
    }

}


void timerButtonMinus()
{   second -= STEP_TIME_SETTING;
    if (second < 0)
    {
        second = 0;
    }

}



void loop()
{
    myTM1637.display("0:00");

    do {

        uint8_t key = myTM1637.readKey();
        switch (key )
        {
            case myTM1637.nokey:
                // do nothing
                continue;

            case myTM1637.key1:
                Serial.print("have key1\n");

                fireAlarmSound();
                timerButtonMinus();
                delay(150);
                stopAlarmSound();
                delay(100);

                break;
            case myTM1637.key2:
                Serial.print("have key2\n");
                fireAlarmSound();
                timerBottonPlus();
                delay(150);
                stopAlarmSound();
                delay(100);
                break;
            case myTM1637.key3:
                Serial.print("have key3\n");
                fireAlarmSound();
                delay(350);
                stopAlarmSound();

                if (second > 0)
                {
                    Timer1.initialize(OneSecond);
                    Timer1.attachInterrupt( timerInterrupt ); // attach the service routine here
                    digitalWrite(pin_relay_delayOff, HIGH);

                }

                break;
            case myTM1637.key4:
                Serial.print("have key4\n");
                //second += 30;
                break;
            default :
                continue;

        }
        

        Serial.print("new second is " + String(second) + "\n");
        if (key != myTM1637.nokey) // number changed
        {

            refreshDisplayTime( second) ;
            //don't take another key in too short time
            delay(300);
        }

    } while (true);



}


