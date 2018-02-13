#include "superRotaryEncoder.hpp"


SuperRotaryEncoder::SuperRotaryEncoder(int pinA, int pinB)
{
   RotaryEncoderA=pinA;
   RotaryEncoderB=pinB; 
   pinMode(pinA, INPUT_PULLUP);
   pinMode(pinB, INPUT_PULLUP);
}




unsigned long SuperRotaryEncoder::getTimeStamp()
{
    //unsigned long  timerMicro = micros();
    unsigned long  timerMillis = millis();
    unsigned long  newTimeStamp = timerMillis;
    return newTimeStamp;

}







int SuperRotaryEncoder::getEncoderValueSum()
{

    const unsigned long  AllowTimeDeltaMillieSec = 50; // key value change only accept if it's larger than this micro second


    static int encoderValue = 100;
    static int valueASum = 0;
    static int valueBSum = 0;
    const int MinSum = 5;

    // begin with A=1 B=1 and end with A=1  B=1
    int valueA = digitalRead( RotaryEncoderA );
    int valueB = digitalRead( RotaryEncoderB );
    static bool started = false;
    if ( (valueA == HIGH) && (valueB == HIGH))
    {
        if (!started)
        {
            //Serial.println("not started, all high");
            //idle, nothing changed
            return encoderValue;

        } else
        {
            //started and HIGH HIGH
            unsigned long timestamp = getTimeStamp();
            static unsigned long oldTimeStamp = 0;
            if (( timestamp - oldTimeStamp ) < AllowTimeDeltaMillieSec )
            {
                //Serial.println("drop");

                //happened too fast, drop
                valueASum = 0;
                valueBSum = 0;
                started = false;
                return encoderValue;
            }
            oldTimeStamp = timestamp;
            //already started, now it's time to end
            if (valueASum > valueBSum)
            {
                //if (valueASum > MinSum)
                {
                    //Serial.println(String(valueASum)+ " vs "+ String(valueBSum));
                    encoderValue++;
                }
            }
            else
            {
                // if (valueBSum > MinSum)
                {
                    if(debug) 
                    {
                       Serial.print("--------"); 
                    }

                    encoderValue--;
                }
            }
            valueASum = 0;
            valueBSum = 0;
            if(debug)
            {
                Serial.println(String(encoderValue));
            }
            started = false;
            return encoderValue;
        }
    } else
    {
        // not all high
        started = true;
        valueASum += valueA;
        valueBSum += valueB;

        return encoderValue;
    }



}




// using sum and acceleration to handle rotary encoder. so far this works the best.
// turn left and right like crazy , 100% accurate.
int SuperRotaryEncoder::getEncoderValueSumAccel()
{

    const unsigned long  AllowTimeDeltaMillieSec = 40; // key value change only accept if it's larger than this micro second
    const int MaxAccel = 2;
    //acceleration
    static int accel = 0;
    static int encoderValue = 100;
    static int valueASum = 0;
    static int valueBSum = 0;
    //const int MinSum = 5;

    // begin with A=1 B=1 and end with A=1  B=1
    int valueA = digitalRead( RotaryEncoderA );
    int valueB = digitalRead( RotaryEncoderB );
    static bool started = false;
    unsigned long timestamp = getTimeStamp();
    static unsigned long oldTimeStamp = 0;

    if ( (valueA == HIGH) && (valueB == HIGH))
    {
        if (!started)
        {
            //Serial.println("not started, all high");
            //idle, nothing changed

            //clear accel when idle for long time
           
            if (( timestamp - oldTimeStamp ) > AllowTimeDeltaMillieSec )
            {
               accel=0;
            }
            return encoderValue;

        } else
        {
            //started and HIGH HIGH
            if (( timestamp - oldTimeStamp ) < AllowTimeDeltaMillieSec )
            {
                //Serial.println("drop");

                //happened too fast, drop
                valueASum = 0;
                valueBSum = 0;
                started = false;
                return encoderValue;
            }
            oldTimeStamp = timestamp;
            //already started, now it's time to end
            if (valueASum > valueBSum)
            {
                if (accel < MaxAccel )
                {
                    accel++;
                }

                if (accel > 0 )
                {
                    encoderValue++;
                }
            }
            else
            {

                if (accel > (-1)*MaxAccel )
                {
                    accel--;
                }

                if (accel < 0 )
                {
                    if(debug) 
                    {
                        Serial.print("--------"); 
                    }
                    encoderValue--;
                }

            }
            valueASum = 0;
            valueBSum = 0;
            if(debug) 
            {
                Serial.println(String(encoderValue)); 
            }
            
            started = false;
            return encoderValue;
        }
    } else
    {
        // not all high
        started = true;
        valueASum += valueA;
        valueBSum += valueB;

        return encoderValue;
    }



}



void SuperRotaryEncoder::displayValues()
{
    int valueA = digitalRead( RotaryEncoderA   );
    int valueB = digitalRead( RotaryEncoderB   );

    if ((valueA == HIGH) && (valueB == HIGH))
    {
        return;
    }
    Serial.println("A=" + String(valueA));

    Serial.println("B=" + String(valueB));

    Serial.println("==========");


}



void SuperRotaryEncoder::selftest()
{
   this->debug=true;
   while(true)
   {   
       int value = this->getEncoderValueSum();
       //          this->getEncoderValueSumAccel();
   };
  
  
}



