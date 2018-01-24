#include "TM1637_Arduino_Chris.hpp"
#include <TimerOne.h>


// Pins connect to arduino
// if use arduino RX, TX (0,1), no pull up resistor is needed, other pins, need pull up resistor for each.
#define pin_CLK A1
#define pin_DIO A0
#define pin_firealarm A3
// if frozen, try to add two 100pf capacitors for each wire

// only connected a 4 digit digitube
#define installDigits  4


const bool debugTM1637 = false;

const long OneSecond = 1000000;


TM1637_Arduino_Chris myTM1637(pin_CLK, pin_DIO, installDigits);

//make the longest timer as one hour -1 second
const long MAXTimerSecond = 60 * 60 - 1;

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


void fireAlarm(void)
{
    digitalWrite(pin_firealarm, LOW);

}


void timerInterrupt()          // timer compare interrupt service routine
{
    if ( second == 0 )
    {

        refreshDisplayTime(second);

        Timer1.stop();

        fireAlarm();
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
    myTM1637.m_debugPrint = debugTM1637;

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    pinMode(pin_firealarm , OUTPUT);
    digitalWrite(pin_firealarm, LOW);

    if (myTM1637.m_debugPrint  ) {
        Serial.begin(9600);      // open the serial port at 9600 bps:
    }
    Serial.begin(9600);
    digitalWrite(LED_BUILTIN, HIGH);

    myTM1637.setBrightness(0);

    myTM1637.clearAll();
    Timer1.stop();
    Timer1.attachInterrupt( timerInterrupt ); // attach the service routine here

}


void timerBottonPlus()
{
    second += 20;
    if (second > MAXTimerSecond )
    {
        second = MAXTimerSecond ;
    }

}


void timerButtonMinus()
{   second -= 20;
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

                timerButtonMinus();
                break;
            case myTM1637.key2:
                timerBottonPlus();

                break;
            case myTM1637.key3:

                if (second > 0)
                {
                    Timer1.initialize(OneSecond);

                    digitalWrite(pin_firealarm, HIGH);
                }

                break;
            case myTM1637.key4:
                //second += 30;
                break;
            default :
                continue;
        }
        //myTM1637.debugPrint("read key is ", key);

        Serial.print("new second is " + String(second) + "\n");
        if (key != myTM1637.nokey) // number changed
        {

            refreshDisplayTime( second) ;
            //don't take another key in too short time
            delay(300);
        }
    } while (true);



}


