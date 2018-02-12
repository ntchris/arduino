
// built in LED
const int LedPin = 13;
const int RotaryEncoderA = 3;
const int RotaryEncoderB = 2;
// Connect pin C to GND
// HOW to determine which PIN is A, which is B, but it deosn't really matter.
// Using  displayValues() function, check serial output window
// rotate left, the pin always first turn to 0 from 1, is A.  the other pin is B.

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.print("Setup \n");

    pinMode(LedPin, OUTPUT);
    pinMode(RotaryEncoderA, INPUT_PULLUP);
    pinMode(RotaryEncoderB, INPUT_PULLUP);

    digitalWrite(LedPin, LOW);

}




// each new value has a time stamp. if new value's time stamp delta with last value 's time stmap is too small
// ignore the new value

//must be put in a loop
unsigned long getTimeStamp()
{
    //unsigned long  timerMicro = micros();
    unsigned long  timerMillis = millis();
    unsigned long  newTimeStamp = timerMillis;
    return newTimeStamp;

}



int getAvgReading(int port, int currentAvgValue)
{
    const int MaxEncoderValue = 2;
    const int MinEncoderValue = -1 * 20 ;


    int value = digitalRead(port);

    if ( value == HIGH)
    {
        if (currentAvgValue < MaxEncoderValue)
        {
            currentAvgValue++;
        }
    } else
    {
        // A reading LOW
        if (currentAvgValue > MinEncoderValue)
        {
            currentAvgValue--;
        }
    }
    return currentAvgValue;
}


int getEncoderValueAvg()
{
    const unsigned long  AllowTimeDeltaMillieSec = 5; // key value change only accept if it's larger than this micro second
    static int encoderValue = 100;

    static unsigned long  oldValueTimeStamp = 0;
    static int avgEncoderA = 0;
    static int avgEncoderB = 0;


    int encoderA, encoderB;
    static int oldEncoderA = HIGH, oldEncoderB = HIGH;
    //default is HIGH for both
    avgEncoderA = getAvgReading(RotaryEncoderA, avgEncoderA);
    avgEncoderB = getAvgReading(RotaryEncoderB, avgEncoderB);


    unsigned long timeStamp = getTimeStamp();
    unsigned long delta = timeStamp - oldValueTimeStamp;
    if (delta < AllowTimeDeltaMillieSec )
    {
        //Serial.print("too short, ignoring\n");
        return encoderValue;
    }
    oldValueTimeStamp = timeStamp;


    //Serial.println("avg A is "+ String(avgEncoderA ));
    //Serial.println("avg B is "+ String(avgEncoderB ));

    //Only check encoder status if pin A has new value

    // Serial.print("avg is "+ String(avgEncoderA )+ String("\n") );

    if (avgEncoderA > 0)
    {
        encoderA = HIGH;
    } else
    {
        encoderA = LOW;
    }

    if (avgEncoderB > 0)
    {
        encoderB = HIGH;
    } else
    {
        encoderB = LOW;
    }

    if ( encoderA == oldEncoderA)
    {
        //Serial.print("new A==oldA, ignore\n") ;
        return encoderValue;
    }

    if (encoderA == HIGH && encoderA != oldEncoderA)
    {

        //update oldEncoderA value
        Serial.print("A==HIGH, update oldEncoderA=HIGH\n");

        /*
                const int Max = 50;
                for (int i = 0; i < Max; i++)
                {
                    avgEncoderB = getAvgReading(RotaryEncoderB, avgEncoderB);
                }
                if (avgEncoderB > 0)
                {
                    encoderB = HIGH;
                } else
                {
                    encoderB = LOW;
                }

                Serial.print("avgB is " + String(avgEncoderB) + String("\n"));
                 avgEncoderB = 0;
        */
        if (encoderB == encoderA)
        {
            Serial.print("------------------");

            encoderValue--;
        } else
        {
            Serial.print("++++++++++++++++");
            encoderValue++;
        }
        oldEncoderA = encoderA;

        Serial.print(encoderValue);
        Serial.print("\n");

    } else if (encoderA == LOW  && encoderA != oldEncoderA )
    {
        Serial.print("A==LOW, update oldEncoderA=LOW\n");
        oldEncoderA = encoderA;
    }
    avgEncoderA = 0;
    avgEncoderB = 0;

    return encoderValue;
}

























// using acceleration.  left left left right left is impossible
int getEncoderValueAccel()
{
    static int encoderValue = 100;

    static unsigned long  oldValueTimeStamp = 0;
    const int MAXACCEL = 5;
    const int MINACCEL = -1;
    static int accel = 0;

    const unsigned long  AllowTimeDeltaMillieSec = 20; // key value change only accept if it's larger than this micro second


    int encoderA, encoderB;
    static int oldEncoderA = HIGH, oldEncoderB = HIGH;
    encoderA = digitalRead(RotaryEncoderA);

    if ( encoderA == oldEncoderA)
    {
        unsigned long timeStamp = getTimeStamp();
        unsigned long delta = timeStamp - oldValueTimeStamp;
        if (delta > AllowTimeDeltaMillieSec )
        {
            oldValueTimeStamp = timeStamp;
            accel = 0;

        }

        return encoderValue;
    }

    if (encoderA == HIGH && encoderA != oldEncoderA)
    {

        //update oldEncoderA value
        Serial.print("A==HIGH, update oldEncoderA=HIGH\n");

        oldEncoderA = encoderA;
        encoderB = digitalRead(RotaryEncoderB);
        Serial.println("B is " + String(encoderB ));
        if (encoderB == encoderA)
        {
            Serial.println("------------------");
            if ( accel > MINACCEL)
            {
                accel--;
            }

        } else
        {
            if (accel < MAXACCEL)
            {
                accel++;
            }
        }

        Serial.println("accel is " + String(accel));
        if ( accel < 0)
        {
            encoderValue--;
        } else
        {
            encoderValue++;
        }

        Serial.println(encoderValue);


    } else if (encoderA == LOW  && encoderA != oldEncoderA )
    {


        oldEncoderA = encoderA;
    }


    return encoderValue;
}





































int getEncoderValueOld()
{
    static int encoderValue = 100;

    static unsigned long  oldValueTimeStamp = 0;
    static int oldEncodeA = HIGH;

    const unsigned long  AllowTimeDeltaMillieSec = 20; // key value change only accept if it's larger than this micro second

    int encodeB;
    int encoderA = digitalRead(RotaryEncoderA);
    //Only check encoder status if pin A has new value

    if ( encoderA == oldEncodeA)
    {
        return encoderValue;
    }




    if (encoderA == HIGH && encoderA != oldEncodeA)
    {

        unsigned long timeStamp = getTimeStamp();
        unsigned long delta = timeStamp - oldValueTimeStamp;
        if (delta < AllowTimeDeltaMillieSec   )
        {

            Serial.print("new A==HIGH, too short, ignoring\n");
            return encoderValue;
        }
        oldValueTimeStamp = timeStamp;
        //update oldEncoderA value
        Serial.print("A==HIGH, update oldEncoderA=HIGH\n");

        oldEncodeA = encoderA;

        encodeB = digitalRead(RotaryEncoderB);
        if (encodeB == encoderA)
        {
            Serial.print("------------------");

            encoderValue--;
        } else
        {
            encoderValue++;
        }

        Serial.print(encoderValue);
        Serial.print("\n");
    } else if (encoderA == LOW  && encoderA != oldEncodeA )
    {

        unsigned long timeStamp = getTimeStamp();
        unsigned long delta = timeStamp - oldValueTimeStamp;
        if (delta < AllowTimeDeltaMillieSec )
        {

            Serial.print("A==LOW, too short, ignoring\n");
            return encoderValue;
        }
        Serial.print("A==LOW, update oldEncoderA=LOW\n");
        oldEncodeA = encoderA;
    }


    return encoderValue;
}





/*

    int getEncoderValue()
    {
    static int encoderValue = 100;

    static unsigned long  oldValueTimeStamp = 0;
    static int oldEncodeA = HIGH;

    const unsigned long  AllowTimeDeltaMillieSec = 30; // key value change only accept if it's larger than this micro second

    static int encoderB = 0;
    int encoderA = digitalRead(RotaryEncoderA);

    const int MIN_B = -50, MAX_B = 1;

    encoderB = digitalRead(RotaryEncoderB);

    if ( digitalRead(RotaryEncoderB) == HIGH )
    {
        if ( encoderB < MAX_B)
        {
            encoderB ++;
        }
    } else
    {
        if ( encoderB  > MIN_B )
        {   encoderB --;
        }
    }


    //Only check encoder status if pin A has new value

    if ( encoderA == oldEncodeA)
    {
        return encoderValue;
    }

    if (encoderA == HIGH && encoderA != oldEncodeA)
    {
        //update oldEncoderA value
        Serial.print("A==HIGH, update oldEncoderA=HIGH\n");

        oldEncodeA = encoderA;

        Serial.println("B is " + String(encoderB ));
        if (encoderB <= 0 )
        {
            encoderB = LOW;
        } else
        {
            encoderB = HIGH;
        }


        unsigned long timeStamp = getTimeStamp();
        unsigned long delta = timeStamp - oldValueTimeStamp;
        if (delta < AllowTimeDeltaMillieSec   )
        {

            Serial.print("new A==HIGH, too short, ignoring\n");
            return encoderValue;
        }
        oldValueTimeStamp = timeStamp;

        if (encoderB == encoderA)
        {

            Serial.print("------------------");

            encoderValue--;
        } else
        {
            Serial.print("++++++++++++++++");

            encoderValue++;
        }

        Serial.print(encoderValue);
        Serial.print("\n");
    } else if (encoderA == LOW  && encoderA != oldEncodeA )
    {

        unsigned long timeStamp = getTimeStamp();
        unsigned long delta = timeStamp - oldValueTimeStamp;
        if (delta < AllowTimeDeltaMillieSec )
        {

            Serial.print("A==LOW, too short, ignoring\n");
            return encoderValue;
        }
        Serial.print("A==LOW, update oldEncoderA=LOW\n");
        oldEncodeA = encoderA;
    }


    return encoderValue;
    }

*/



int getEncoderValueSum()
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
                if (valueASum > MinSum)
                {
                    //Serial.println(String(valueASum)+ " vs "+ String(valueBSum));
                    encoderValue++;
                }
            }
            else
            {
                // if (valueBSum > MinSum)
                {
                    Serial.print("--------");

                    encoderValue--;
                }
            }
            valueASum = 0;
            valueBSum = 0;
            Serial.println(String(encoderValue));
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
int getEncoderValueSumAccel()
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
                    Serial.print("--------");
                    encoderValue--;
                }

            }
            valueASum = 0;
            valueBSum = 0;
            Serial.println(String(encoderValue));
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



void displayValues()
{
    int valueA = digitalRead( RotaryEncoderA   );
    int valueB = digitalRead( RotaryEncoderB   );

    if ((valueA == HIGH) && (valueB == HIGH))
    {
        return;
    }
    Serial.println("A=" + String(valueA));

    Serial.println("B=" + String(valueB));

    Serial.println("==");


}



void loop() {
    // put your main code here, to run repeatedly:

    digitalWrite(LedPin, LOW);

    //    int encoderValue = getEncoderValueAvg();
    //Serial.print(encoderValue);
    //Serial.print("\n");

    displayValues();
    
    //getEncoderValueSum();
 //   getEncoderValueSumAccel();

    
    //digitalWrite(ledPin, HIGH);
    //delay(500);

}
