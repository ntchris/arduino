#include <TFT.h>

// DIY Weller solder pen controller.
// kty cold junction compensation is not done to reduce the project's complexicity; and I don't think it's important.
// magnet detection is included.
//

// this is for the PCB  March 10 2018
#include "Super_TM1637_Arduino.hpp"

#include "superRotaryEncoder.hpp"

// ******************    Pin definition   ******************
const int HeaterEnableABrown = A3;  // Heater2, Brown
const int HeaterEnableARed = A4;    // Heater1, Red
const int OpampOutput1ForPurplePin = A5;//detect voltage for purple temp after opamp. For Red heater
const int OpampOutput2ForBlackPin = A2; // detect voltage for black temp after opamp. For brown heater
const int MagnetDetectPin = A1; // check voltage to see if magnet switch is connected , if so, it's put on handle rest. should shutdown

// magnet detection: pen on handle, magnet make pin 1 gnd and pin 6 purple connected/shorted,  not on handle(using), r=2K.
const int MagnetDetectEnablePin = 6; // D6 Magnet Pullup  , provide a 2.5V to check magnet switch for a short time
const int RotaryButton = 12;
const int RotaryPinA = 10;
const int RotaryPinB = 11;
const int I2C_SDA = 2;  // D2, for TM1637 display
const int I2C_SCL = 3;  // D3, for TM1637 display

const int KtyEnablePin = 5;  // D5 , not useful in this version, aka KtyPullup
const int TcInputBlackKty = A6;  //  not useful in this version, aka Kty input

// ******************    End of Pin definition   ******************



//===========================================
const int EnvTempC = 18;

const float ADC1024 = 1024.0;
const float VCC5 = 4.97;
const float NotConnectedVoltage = 4.9;
const int NotConnectedVoltageInt = 999;  // ADC, if 5.0v then it's 1023


const float OpAmpR1Black = 0.991;//1 KOhm
const float OpAmpR2Purple = 0.991; //  1 KOhm
const float OpAmpRopamp2A = 510.0;  //TC Black  // 510K , R 514
const float OpAmpRopamp2B = 510.0;  //TC Black  // 510K , R 514
const float OpampGainPurple = OpAmpRopamp2A  / OpAmpR2Purple + 1.0;
const float OpampGain = OpampGainPurple ;

//  const float opampGainBlack = OpAmpRopamp2B * 1.0 / OpAmpR1Black + 1.0;
// For simplicity, assume opamp gain for input purple and input black are the same,


const int TCuVPerC = 12;  //41uV per C  thermal couple type K.

const int MIN_TARGET_TEMP = 35;
const int Default_Target_Temp = 200;

// adjust the reading to make two heating element about the same temperature
const int DeltaAnalogReadIntDeltaForPurple= 0; 
const int DeltaTempForPurple= 35; 

//===============================================================================
// ****************  For display TM1637 and seven segment digitube  *****************
const int InstallDigits = 3;
Super_TM1637_Arduino myTM1637(I2C_SCL, I2C_SDA, InstallDigits);


SuperRotaryEncoder rotEncoder(RotaryPinA, RotaryPinB);



/**
     note: must clean up all flux otherwise ADC reading could be 0 or low while not connected
     (but it should be constantly high or 1023 or 4.9 )
     not connect float 4.9v  int 1023

    R 1.2K   and 220K
    hand temp  0.05V  (opamp pin 1 output)
    XXXXXX  wrong  room temp  float 0.91v  int 187  ????    wrong
    solder not melt temp  float 0.21v  int 42  ????

    both :  room temp =0
    hand:   int =1 , vol =0
    measure 167C,    YiHua 100C (heat gun)  int=38,  vol=0.19
    100C (heat gun)  int=77,  vol=0.38   2:55pm  Feb18 2018



    61C , 0.11 gained,
    solder melt  int 146 0.7V    95C (wrong ??)

                         63       0.31  (not melt solder)
         150C            98,  0.48


    200C (   )  int=136,  vol=0.66
    250            189        0.92
    300            285        1.39

*/







//=======================================================
class WellerSolderControllerStatus
{
    public:
        int purpleInt;
        int blackInt;
        bool isPurpleConnected;
        bool isBlackConnected;
        bool isConnected;
        bool isOnRest;
};

WellerSolderControllerStatus myWellerSolderController;



// only for reading int value from purple and black pin's value
bool opampReadIntToIsConnected(int opampValInt )
{
    //if not connected, the opamp output is high 5.0V
    if ( opampValInt >= NotConnectedVoltageInt  )
    {
        return false;;
    } else
    {
        //Serial.println("opampReadIntToIsConnected opampValInt " + String(opampValInt ) + " false");
        return true;
    }
}


int getAnalogAvgReadingInt(int analogReadPin, int avgCount)
{

    int i, opampValInt = 0;
    for (i = 0; i < avgCount; i++)
    {
        opampValInt += analogRead(analogReadPin);
        delayMicroseconds(600);
    }
    opampValInt = opampValInt / avgCount;
    return opampValInt;
}

 
 

void pinInit()
{

    pinMode(HeaterEnableARed, OUTPUT );
    digitalWrite(HeaterEnableARed, LOW);


    pinMode(HeaterEnableABrown, OUTPUT );
    digitalWrite(HeaterEnableABrown, LOW);


    pinMode(OpampOutput1ForPurplePin, INPUT );
    pinMode(OpampOutput2ForBlackPin, INPUT );
    pinMode(MagnetDetectEnablePin, INPUT);

}

void displayInit()
{
    const int DefaultBrightness = 3;

    myTM1637.setBrightness(DefaultBrightness);

    myTM1637.display("S.t.b.");
}

void enableHeater(int heaterpin)
{
    digitalWrite(heaterpin, HIGH);

}


void disableHeater(int heaterpin)
{
    digitalWrite(heaterpin, LOW);
}

void disableAllHeaters()
{
    disableHeater(HeaterEnableABrown);
    disableHeater(HeaterEnableARed);

}



bool isPenPutOnRest()
{
    const int MagnetDetectValue = 15;
    //  when on rest, magnet connect to ground, reading should be low , close to 0.
    //  when on use , magnet connect to 2K then to ground, reading should be higher than 0.

    disableAllHeaters();
    delayMicroseconds(500);
    pinMode(MagnetDetectEnablePin, OUTPUT ); // when output conflict with opamp pin7 output

    digitalWrite(MagnetDetectEnablePin, HIGH);
    delayMicroseconds(200);

    int magnetReadInt = analogRead(MagnetDetectPin);

    //when magnet switch is close, magnet detect pin connects to gnd, so it's low

    //Serial.println("magnet detect pin avg: " + String(magnetReadInt) );


    digitalWrite(MagnetDetectEnablePin, LOW);

    pinMode(MagnetDetectEnablePin, INPUT); // when output conflict with opamp pin7 output
    delay(2);
    bool isOnRest = (magnetReadInt < MagnetDetectValue );
    return isOnRest;
}





//The opamp voltage to temp
// measured temp  -->
// 41uV/C
int voltageToTempC(float vol, int envTempC = 18)
{
    float thermalJunction_mV = (vol * 1000.0 ) / OpampGain;
    //Serial.println("thermalJunction_mV: " + String(thermalJunction_mV));
    //assume env is always 20C for now
    float envTCVoltage_mv = envTempC * TCuVPerC / 1000; // envTempC voltage ,  uV   18C

    float v1_mv = envTCVoltage_mv + thermalJunction_mV;
    int JunctionTemp = v1_mv * 1000.0 / TCuVPerC ;
    return JunctionTemp;

}



void showADCValue(String info, int opampAdcInt)
{
    static int oldOpAmpInt = -1;

    if ( abs( oldOpAmpInt - opampAdcInt) >= 15  )
    {
        float voltage = VCC5 * opampAdcInt / ADC1024;

        //Serial.println("opampValInt:" + String(opampAdcInt) + ", Voltage:" + String(voltage)  );

        float temp = voltageToTempC(voltage, 18 );

        Serial.println( info + " " + String(temp) );

        oldOpAmpInt = opampAdcInt;
    }


}

void setup() {
    // put your setup code here, to run once:

    pinInit();
    Serial.begin(9600);
    Serial.println("Starting");
    displayInit();


}



/**

     Thermal Couple K
     41uV/C ,  410uV /10C, 820uV/20C, 2050uV/50C,  4100uV/100C
     8200uV/200C ,    12mv/300C,    16mv/400C,  20.5mv/500C

     Must turn heater OFF before check temp.
*/
bool zzzisTempTooHigh(float opampVoltageLimit)
{
    disableHeater(HeaterEnableARed);
    delay(5);
    int opampValInt = analogRead(OpampOutput1ForPurplePin);

    showADCValue("zzz", opampValInt );

    float voltage = VCC5 * opampValInt / ADC1024;

    if ( voltage >= opampVoltageLimit )
    {
        return true;
    }
    else
    {

        return false;
    }

}

int getPurpleOpAmpOutputInt()
{
    disableHeater( HeaterEnableARed );
    delay(4);
    int purpleInt = getAnalogAvgReadingInt(OpampOutput1ForPurplePin, 3);
    //Serial.println( "purpleInt before:"+String(purpleInt));
    purpleInt = purpleInt  + DeltaAnalogReadIntDeltaForPurple;
    //Serial.println( "purpleInt after:"+String(purpleInt));
    return purpleInt;
}

int getBlackOpAmpOutputInt()
{
    disableHeater( HeaterEnableABrown );
    delay(4);
    int blackInt = getAnalogAvgReadingInt(OpampOutput2ForBlackPin, 3);
    return blackInt;
}


/*
    int getRedTemperature()
    {
    //disableHeater(HeaterEnableARed);
    //delay(2);
    int measuretemp = getOpampOutputTemperature(OpampOutput1ForPurplePin);!!!!!
    // Serial.println("red measured temp: " + String(measuretemp));
    return measuretemp;

    }


    int getBrownTemperature()
    {
    disableHeater(HeaterEnableABrown);
    delay(2);
    int measuretemp = getOpampOutputTemperature(OpampOutput2ForBlackPin);
    // Serial.println("brown measured temp: " + String(measuretemp));
    return measuretemp;

    }
*/


// must disable heater first and delay!!
int getOpampOutputIntToTemperature(int analogReadingInt)
{
    /*  const int MAX = 4;
        int i;
        int opampAdcInt;
        for (i = 0; i < MAX; i++)
        {
         opampAdcInt += analogRead(OpampOutput1ForPurplePin);
         delayMicroseconds(500);
        }
        opampAdcInt = opampAdcInt / MAX;
    */
    static int  measured_temp = 0;
    const int MAX_VALID_INT=700;
    if ( analogReadingInt > MAX_VALID_INT )
    {
       return measured_temp;   
    }
    float voltage = VCC5 * analogReadingInt * 1.0 / ADC1024;

    measured_temp = voltageToTempC(voltage, 18);
    return measured_temp;
}


/*
    // , float opampVoltage, int envTempC
    bool isTempTooHigh_temp(int target_temp)
    {
    disableHeater(HeaterEnableARed );
    delay(3);

    const float MAX = 5.0;
    int i;
    float opampAdcInt;
    for (i = 0; i < MAX; i++)
    {
        opampAdcInt += analogRead(OpampOutput1ForPurple);
        delay(1);
    }
    opampAdcInt = opampAdcInt / MAX;

    showADCValue(opampAdcInt );

    float voltage = VCC5 * opampAdcInt / ADC1024;

    float measured_temp = voltageToTempC(voltage, 18);
    if ( measured_temp > target_temp )
    {
        //Serial.println("measured temp: " + String(measured_temp));
        return true;
    } else
    {
        return false;
    }
    }
*/

int getTargetTemperature()
{
    static int targetTeperature = Default_Target_Temp;
    static int old_rotaryValue = 100;
    int rotaryValue = rotEncoder.getEncoderValueSumAccel();
    if ( rotaryValue  != old_rotaryValue )
    {
        const int RotaryToTempDeltaTimes = 5;
        int delta = rotaryValue - old_rotaryValue;
        targetTeperature = targetTeperature + delta * RotaryToTempDeltaTimes;
        if (targetTeperature < MIN_TARGET_TEMP)
        {
            targetTeperature = MIN_TARGET_TEMP;
        }
        //Serial.println("setting target temp: " + String(targetTeperature));
        old_rotaryValue = rotaryValue;
    }

    //Serial.println("targetTeperature: " + String(targetTeperature)  );

    return targetTeperature;
}


void refreshDisplayFlashing(int digit)
{
    static int flashingcounter = 0;
    static bool showDecimal = true;
    const int Max = 2;

    if (flashingcounter > Max)
    {
        flashingcounter = 0;
        showDecimal = !showDecimal;

        if (showDecimal)
        {
            myTM1637.display( String(digit) + (".") );

        } else
        {
            myTM1637.display( String(digit));
        }

    }

    flashingcounter++;


}




void loop() {

    static unsigned int actionCounter = 0;
    const unsigned int ActionCounterMax = 65535;
    static int oldval = 0;
    int targetTemp = getTargetTemperature(); //from rotary encoder

    // put your main code here, to run repeatedly:
    
    if (   actionCounter >= ActionCounterMax  )
    {
        actionCounter  = 0;
    }
    actionCounter++;
    if ( actionCounter % 50 )
    {
       myTM1637.display(String(targetTemp));

      return;
    }
     
        myWellerSolderController.purpleInt = getPurpleOpAmpOutputInt();
        myWellerSolderController.isPurpleConnected = opampReadIntToIsConnected(myWellerSolderController.purpleInt);

        myWellerSolderController.blackInt = getBlackOpAmpOutputInt();
        myWellerSolderController.isBlackConnected = opampReadIntToIsConnected(myWellerSolderController.blackInt);


        if ( !myWellerSolderController.isPurpleConnected && !myWellerSolderController.isBlackConnected )
        {
            //Serial.println("purple connected is " + String(purple_connected));
            //Serial.println("black connected is " + String(black_connected));

            myWellerSolderController.isConnected = false;
        } else
        {
            myWellerSolderController.isConnected = true;
        }


        if (myWellerSolderController.isConnected)
        {
            myWellerSolderController.isOnRest = isPenPutOnRest();
            if ( myWellerSolderController.isOnRest )
            {
                //already disbled ??
                disableAllHeaters();
                myTM1637.display("OFF");
                return;
            }
        } else
        {
            myTM1637.display("dsc");
            return;
        }
   

    if (myWellerSolderController.isConnected )
    {
        //  ==================   check Red   ===================
        //int redtemp = getRedTemperature();!!!!!
        int redtemp = getOpampOutputIntToTemperature(myWellerSolderController.purpleInt) + DeltaTempForPurple;
        
        if(myWellerSolderController.purpleInt>500)
        {
          showADCValue("!!!!!!!!!!!purple int!!!!!!!!!!! ", myWellerSolderController.purpleInt );
        }else
        {
            showADCValue("purple int ", myWellerSolderController.purpleInt );
        }

        bool is_too_high_red = redtemp > targetTemp;
        if ( is_too_high_red )
        {
            // turning off
            // but should have been disabled at this point in the get temp function
            disableHeater(HeaterEnableARed);

        } else
        {
            // not too high, still need heating , turn on heater
            Serial.println("red heating on!!! red temp " + String(redtemp) );
            enableHeater(HeaterEnableARed);
        }


        //  ==================   check brown   ===================
        int browntemp = getOpampOutputIntToTemperature(myWellerSolderController.blackInt);
        showADCValue("brown int", myWellerSolderController.blackInt );

        bool is_too_high_brown = browntemp > targetTemp;
        if ( is_too_high_brown )
        {
            // turning off
            // but should have been disabled at this point in the get temp function
            //Serial.println("brown heating offff!");

            disableHeater(HeaterEnableABrown);

        } else
        {
            // not too high, still need heating , turn on heater
            Serial.println("brown heating on! brown temp " + String(browntemp));
            enableHeater(HeaterEnableABrown);
        }


        if ( is_too_high_red && is_too_high_brown)
        {
            // both temperature is good now.
            myTM1637.display(String(targetTemp));
        } else
        {
            //still heating , show flashing point
            //Serial.println("flashing!");
            refreshDisplayFlashing(targetTemp);
        }


    }
 



    //delay(1);


}
